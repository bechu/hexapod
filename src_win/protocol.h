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
	void dump(uint8_t c);
    void _write(uint8_t *data, int size);
	uint8_t _read();
	UartHW *_uart;
	static const int BUFFER_SIZE = 20;
	uint8_t in[BUFFER_SIZE];
	uint8_t out[BUFFER_SIZE];
	uint8_t in_offset;
	uint8_t out_offset;
	bool haveMessage;
};

#endif
