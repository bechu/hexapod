#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "hardware.h"

class Packet
{
public:
	Packet();
	char cmd_id;
	char motor_id;
	DRIVE_SPEED pos;
	int16_t t;
	char status;

	typedef enum
	{
		MOVING,
		STOP
	} motor_status;

	typedef enum{
	SET_POS = 1,
	GET_POS = 2,
	GET_STATUS = 3,
	STOP_MOTION = 4	
	} command_type;


	uint8_t command_len[STOP_MOTION+1];
};


class Protocol
{
public:
	Protocol();
	void init(UartHW *uart);
	int treatIO(Packet &p);
	void send(Packet &p);
private:
    void _write(uint8_t *data, int size);
	uint8_t _read();
	UartHW *_uart;
	void print(int16_t i);
	void forge(Packet &p);
	static const int BUFFER_SIZE = 20;
	uint8_t in[BUFFER_SIZE];
	uint8_t out[BUFFER_SIZE];
	uint8_t in_offset;
	uint8_t out_offset;
	bool haveMessage;
};



#endif
