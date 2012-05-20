#include "hardware.h"
#include "protocol.h"
#include "smart_motor.h"


Protocol test;

SmartMotor test_motor(servo1);


// Initialise the hardware
void appInitHardware(void) {
	initHardware();
}


// Initialise the software
TICK_COUNT appInitSoftware(TICK_COUNT loopStart) {

	test.init(&uart1);

	servo1.setSpeed(0);

	return 0;
}


// This is the main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart) {
	Packet p;
	int success;
	if(test.treatIO(p) == 1)
	{
		if(p.motor == 1)
		{
			test_motor.set_position(p.pos, 1000);
		}
	}
	test_motor.compute();
  	return HEXAPOD_LOOP_DURATION;
}
