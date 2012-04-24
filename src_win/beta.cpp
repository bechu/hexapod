#include "hardware.h"
#include "protocol.h"
#include "services.h"

Protocol test;
PingService ping;

// Initialise the hardware
void appInitHardware(void) {
	initHardware();
}


// Initialise the software
TICK_COUNT appInitSoftware(TICK_COUNT loopStart){
	uart1.setPollingMode(false);
	test.init(&uart1);
    ping.init(&test);
	return 0;
}


// This is the main loop
TICK_COUNT appControl(LOOP_COUNT loopCount, TICK_COUNT loopStart) {

    
	// treat input/output for the protocol
	test.treatIO();
//	test.receive();
//	marquee.print("3");
	//if(test.haveMessage == true)
	//		test._write("true", 4);
  /*  if(test.receive() == true)
    {
		test._write("abc", 3);
     //   if(ping.match(msg))
       // {
         //   ping.execute(msg);
        //}
    }*/

/*
	if(uart1.isRxBufferEmpty() == true)
	{
		int r = uart1.read();
		uart1.write(9999);
	}

	// To test if it is pressed then
	if(button.pressed()){
		// pressed
	}
	
	// To test if it is released then
	if(button.released()){
		// released
	}
	
	// -------- End   Switch/Button-------

	// -------- Start Marquee-------
	{		
		// A marquee is a 'Stream' so can use the print calls - 
		// but need to end with '\n' to start scrolling
	
		// If the endDelay is non-zero then the marquee will scroll
		// forever or until you call: marquee.stop();
		
		// If the endDelay is zero then the marquee will stop once
		// the entire line has been shown ('one-shot' mode)
		
		// In 'one-shot' mode then you may want to make sure that
		// a previous line has finished before you display a second line.
		// This can be done as follows:-
		marquee.setEndDelay(0); // Make sure we are in one-shot mode
		if(marquee.isActive()==FALSE){
		     if(loopCount==1){
		     	marquee.print_P(PSTR("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"));
		     }else{
				marquee.print_P(PSTR("Loop="));
				marquee.print(loopCount);
				marquee.println();
		     }
		}
			
	}
	// -------- End   Marquee-------
	*/
	// -------- Start Actuators -------
	// This example will move the motors back and forth using the loopStart time:
	
/*	
	TICK_COUNT ms = loopStart / 1000;		// Get current time in ms
	int16_t now = ms % (TICK_COUNT)10000; 	// 10 sec for a full swing
	if(now >= (int16_t)5000){				// Goes from 0ms...5000ms
		now = (int16_t)10000 - now;			// then 5000ms...0ms
	}
	// Map it into DRIVE_SPEED range
	DRIVE_SPEED speed = interpolate(now, 0, 5000, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX);
	// Set speed for all motors/servos
	servo1.setSpeed(speed);
	servo2.setSpeed(speed);
	servo3.setSpeed(speed);
	// -------- End   Actuators -------
*/
	return 0;
}
