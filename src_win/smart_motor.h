#ifndef SMARTMOTOR_H
#define SMARTMOTOR_H

#include "hardware.h"


class SmartMotor
{
public:
	SmartMotor(Servo &servo);
	
	void set_limit(DRIVE_SPEED min, DRIVE_SPEED max);

	void set_position(DRIVE_SPEED position, int16_t t); // t en ms

	bool is_moving();

	void compute();

private:
	Servo *_servo;
	DRIVE_SPEED _limit_min; //limit min position (due to environemental constrain) in DRIVE_SPEED range 
	DRIVE_SPEED _limit_max; //limit max position (due to environemental constrain) in DRIVE_SPEED range 

	int16_t _position;
	int16_t _step_size;
	uint16_t _step_count;

	static const int16_t SMART_SPEED = 30000;

};

// Map it into DRIVE_SPEED range
//DRIVE_SPEED speed = interpolate(now, 0, 2500, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
//HEXAPOD_LOOP_DURATION
#endif // SMARTMOTOR_H
