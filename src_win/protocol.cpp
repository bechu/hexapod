#include <stdio.h>
#include <string.h>
#include "protocol.h"

Packet::Packet()
{
	motor = 0;
	pos = 0;
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


int Protocol::treatIO(Packet &p)
{	
	_read();
	if(in_offset>=2)
	{
		p.motor = in[0];
		p.pos = in[1];
		in_offset-=2;
		for(uint8_t i=0;i<in_offset;i++)
		{
			in[i] = in[i+2];
		}
		return 1;
	}
	return 0;
}

//////////////////////////////////////////
//
// PRIVATE
//////////////////////////////////////////////
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

