#include "smart_motor.h"


SmartMotor::SmartMotor(Servo &servo)
{

	_servo = &servo;

	_limit_min = DRIVE_SPEED_MIN;
	_limit_max = DRIVE_SPEED_MAX;

	_position = 0;
	_step_size = 0;
	_step_count = 0;
}


void SmartMotor::set_limit(DRIVE_SPEED min, DRIVE_SPEED max)
{
	_limit_min = min;
	_limit_max = max;
}


void SmartMotor::set_position(DRIVE_SPEED position, int16_t t)
{
	position = (position < _limit_min)? _limit_min : position;
	position = (position > _limit_max)? _limit_max : position;

	int16_t pos = 10000; //interpolate(position, -127, 127, -SMART_SPEED, SMART_SPEED);
	

	_step_size = 1;// ((pos - _position) / t) * (HEXAPOD_LOOP_DURATION / 1000);
	_step_count = 100;//(pos - _position) / _step_size;	
	
}

bool SmartMotor::is_moving()
{
	return _step_count != 0;
}


void SmartMotor::compute()
{
	if(_step_count > 0)
	{
		_position = _position + _step_size;
		int16_t i = interpolate(_position, -SMART_SPEED, SMART_SPEED, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
		char str[10];
		for(int j=0;j<10;j++)
			str[j] = 0;
		char *p = str;
		p=itoa(_position, (char*)str, 10);
		for(int j=0;j<10;j++)
		{
			uart1.write(p[j]);
		} 
			uart1.write('\r');
			uart1.write('\n');
	//	_servo->setSpeed(interpolate(_position, -SMART_SPEED, SMART_SPEED, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX));
		_step_count -= 1;
	}

}
// Map it into DRIVE_SPEED range
//DRIVE_SPEED speed = interpolate(now, 0, 2500, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
//HEXAPOD_LOOP_DURATION
//int16_t _position;
//int16_t _step_size;
//uint16_t _step_count;
//SMART_SPEED
