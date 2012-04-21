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
	_read();
	if(in_offset >0 )
{
_write((uint8_t*)"test\n", 5);
dump(in[0]);
_write(in, in_offset);
        	in_offset = 0;
}
    /*if(in_offset >= 2)
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
    }*/
}

#define DUMP(X, Y) 	if(X & Y) \
		*_uart<<'1';\
	else\
		*_uart<<'0';

void Protocol::dump(uint8_t c)
{
	DUMP(c, 0x80);
	DUMP(c, 0x40);
	DUMP(c, 0x20);
	DUMP(c, 0x10);

	*_uart<<' ';

	DUMP(c, 0x08);
	DUMP(c, 0x04);
	DUMP(c, 0x02);
	DUMP(c, 0x01);

	*_uart<<'\n';
}

bool Protocol::receive()
{
/*
if(in_offset == 7 )
{
_write(in, in_offset);
	for(int i=0;i<7;i++)
	{
		dump(i);
	}
//_write(in, in_offset);
//_uart->write(in[0]+'a');
        	in_offset = 0;
};*/
   /* if(in_offset >= 2)
    {
        msg.command = in[0];
        msg.len = in[1];
		if(in_offset > msg.len)
		{
        	for(int i=0;i<msg.len;i++)
        	{
        	    msg.data[i] = in[i+2];
        	}
        	in_offset = 0;
			_write("true", 4);
        	return true;
		}
    }*/
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
	CRITICAL_SECTION{
	while(in_offset < Protocol::BUFFER_SIZE )
	{
		c = _uart->read();
		if(c == -1)
			break;
		in[in_offset] = 0;
		in[in_offset] = (uint8_t)(c & 0xFF);
		in_offset++;
	}
	}
	return 0;
}

