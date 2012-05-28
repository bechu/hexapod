#include <stdio.h>
#include <string.h>
#include "protocol.h"

#include <stdlib.h>


Packet::Packet()
{
	cmd_id = 0;
	motor_id = 0;
	pos = 0;
	t = 0;

	status = Packet::STOP;

	//command_len = {5, 2, 2, 2};
}

Protocol::Protocol()
{
	_uart = NULL;
	in_offset = 0;
	out_offset = 0;
	memset(in, 0, Protocol::BUFFER_SIZE);
	memset(out, 0, Protocol::BUFFER_SIZE);
	haveMessage = false;
}

void Protocol::init(UartHW *uart)
{
	_uart = uart;
	_uart->setPollingMode(false);
}

void Protocol::print(int16_t i)
{
		char str[10];
		for(int j=0;j<10;j++)
			str[j] = 0;
		char *p = str;
		p=itoa(i, (char*)str, 10);
		for(int j=0;j<10;j++)
		{
			_uart->write(p[j]);
		} 
			_uart->write('\r');
			_uart->write('\n');
}



int Protocol::treatIO(Packet &p)
{	
	const static uint8_t command_len[] = {BUFFER_SIZE, 5, 1, 1, 1};
	
	_read();
	uint8_t len = BUFFER_SIZE;
	if(in_offset>0)
	{
		if(in[0] == 0 || in[0] >= sizeof(command_len)/sizeof(uint8_t))
		{
			in_offset = 0;
			return 0;
		}
	//	if(in[0]<sizeof(command_len))
		len = command_len[in[0]];

		uart1<<"len : "<<len;
		
		if (in_offset>=len)
		{
			forge(p);
			
			in_offset-=len;
			for(uint8_t i=0;i<in_offset;i++)
			{
				in[i] = in[i+len];
			}
			return 1;
		}
	}
	if(out_offset>0)
	{
		_write(out, out_offset);
		out_offset = 0;
	}
	return 0;
}

//////////////////////////////////////////
//
// PRIVATE
//////////////////////////////////////////////

void Protocol::send(Packet &p)
{
	switch (p.cmd_id)
	{
		case Packet::SET_POS :{
			break;
		}
		case Packet::GET_POS: {
			out[out_offset++] = p.cmd_id;
			out[out_offset++] = p.motor_id;
			out[out_offset++] = p.pos;
			break;
		}
		case Packet::GET_STATUS:{
			out[out_offset++] = p.cmd_id;
			out[out_offset++] = p.motor_id;
			out[out_offset++] = p.status;
			break;
		}
		case Packet::STOP_MOTION: { 
			break;
		}
	}	
}

void Protocol::forge(Packet &p)
{
	switch (in[0])
	{
		case Packet::SET_POS :{
			p.cmd_id = in[0];
			p.motor_id = in[1];
			p.pos = in[2];
			p.t = (in[3]<<8 & 0xFF00) | (in[4] & 0x00FF);
			break;
		}
		case Packet::GET_POS: {
			p.cmd_id = in[0];
			p.motor_id = in[1];
			uart1<<" command "<<(int)in[0];
			break;
		}
		case Packet::GET_STATUS:{
			p.cmd_id = in[0];
			p.motor_id = in[1];
			break;
		}
		case Packet::STOP_MOTION: { 
			p.cmd_id = in[0];
			p.motor_id = in[1];
			break;
		}
	}	
}


void Protocol::_write(uint8_t *data, int size)
{
	uint8_t *p = data;
	while(p-data < size)
	{
		_uart->write(*p);
		p++;
	}
}

uint8_t Protocol::_read()
{
	int c = -1;
	//CRITICAL_SECTION{
	while(in_offset < Protocol::BUFFER_SIZE )
	{
		c = _uart->read();
		if(c == -1)
			break;
		in[in_offset] = 0;
		in[in_offset] = (uint8_t)(c & 0xFF);
		in_offset++;
	}
	//}
	return 0;
}

