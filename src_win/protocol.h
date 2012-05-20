#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "uart.h"

class Packet
{
public:
	Packet();
	char motor;
	char pos;
};

class Protocol
{
public:
	Protocol();
	void init(UartHW *uart);
	int treatIO(Packet &p);
private:
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
