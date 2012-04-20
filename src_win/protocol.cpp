#include <stdio.h>
#include <string.h>
#include "protocol.h"

Packet::Packet()
{
	command = PROT_ERROR;
	len = 0;
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
}

void Protocol::treatIO()
{
	in_offset += _read(in+in_offset, Protocol::BUFFER_SIZE-in_offset);
    if(in_offset >= 2)
    {
        msg.command = in[0];
        msg.len = in[1];
        for(int i=0;i<msg.len;i++)
        {
            msg.data[i] = in[i+2];
        }
        in_offset = 0;
		haveMessage = true;
    }
	else
		haveMessage = false;
	//if(in_offset >= Protocol::BUFFER_SIZE)
	//	_write("buffer full", 11);
    if(out_offset > 0)
    {
        _write(out, out_offset);
        out_offset = 0;
    }
}

bool Protocol::receive()
{
    if(in_offset >= 2)
    {
        msg.command = in[0];
        msg.len = in[1];
        for(int i=0;i<msg.len;i++)
        {
            msg.data[i] = in[i+2];
        }
        in_offset = 0;
		_write("true", 4);
        return true;
    }
	return false;
}

void Protocol::send(Packet &msg)
{
    if((out_offset + msg.len + 2) < Protocol::BUFFER_SIZE)
    {
        out[out_offset++] = msg.command;
        out[out_offset++] = msg.len;
        for(int i=0;i<msg.len;i++)
            out[out_offset++] = msg.data[i];
    }
}

//////////////////////////////////////////
//
// PRIVATE
//////////////////////////////////////////////
void Protocol::_write(char *data, int size)
{
	char *p = data;
	while(p-data < size)
	{
		_uart->write(*p);
		p++;
	}
}

char Protocol::_read(char *data, int size)
{
	char *p = data;
	char cpt = 0;
	while(_uart->isRxBufferEmpty() == false && p < data+size)
	{
		*p = _uart->read();
		p++;
		cpt++;
	}
	return cpt;
}

