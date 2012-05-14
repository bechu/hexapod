#include "hardware.h"
#include "protocol.h"
#include "services.h"
#include "smart_motor.h"


Protocol test;
PingService ping;

SmartMotor test_motor(servo1);


// Initialise the hardware
void appInitHardware(void) {
	initHardware();
}


// Initialise the software
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){
	//uart1.setPollingMode(false);
	test.init(&uart1);
	servo1.setSpeed(0);

	test_motor.set_position(50,5000);

    //ping.init(&test);
	return 0;
}


// This is the main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart) {

	test_motor.compute();
  	return HEXAPOD_LOOP_DURATION;
}
