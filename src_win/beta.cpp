#include "hardware.h"
#include "protocol.h"
#include "smart_motor.h"


Protocol proto;

SmartMotor motors[2];

// Initialise the hardware
void appInitHardware(void) {
	initHardware();
}


// Initialise the software
TICK_COUNT appInitSoftware(TICK_COUNT loopStart) {

	proto.init(&uart1);

	motors[0].link_servo(servo1);

	motors[1].link_servo(servo2);

	return 0;
}

void treat_packet(Packet &p){

	if(p.motor_id >= sizeof(motors)/sizeof(SmartMotor))
	{
		uart1<<"failed treat packet "<<(int)p.motor_id<<" "<<sizeof(motors)/sizeof(SmartMotor);
		return ;
	}
	switch (p.cmd_id)
	{
		case Packet::SET_POS: { 
			motors[p.motor_id].set_position(p.pos,p.t);
			break;
		}
		case Packet::GET_POS: {
		//	motors[p.motor_id].get_position(p.pos);
			//proto.send(p);
			break;
		}
		case Packet::GET_STATUS:{
			p.status = motors[p.motor_id].is_moving();
			//proto.send(p);
			break;
		}
		case Packet::STOP_MOTION: { 
			motors[p.motor_id].stop_motion();
			break;
		}
	}

}

// This is the main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart) {

	Packet p;

	while(proto.treatIO(p) == 1)
	{
		treat_packet(p);
	}

	for (int i = 0; i < sizeof(motors)/sizeof(SmartMotor); i++)
	{
		motors[i].compute();
	}

  	return HEXAPOD_LOOP_DURATION;
}
