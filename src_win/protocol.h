#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "uart.h"

#define PROT_ERROR 1
#define PROT_PING 2

class Packet
{
public:
	Packet();
	
	static const int MESSAGE_SIZE = 10;

	char command;
	char len;
	char data[MESSAGE_SIZE];
};

class Protocol
{
public:
	Protocol();
	void init(UartHW *uart);
	void treatIO();
    bool receive();
    void send(Packet &msg);
	Packet msg;
//private:
    void _write(char *data, int size);
	char _read(char *data, int size);
	UartHW *_uart;
	static const int BUFFER_SIZE = 20;
	char in[BUFFER_SIZE];
	char out[BUFFER_SIZE];
	char in_offset;
	char out_offset;
	bool haveMessage;
};

#endif
