 /*
 * $Id: servoPWM.c,v 1.9 2010/10/06 12:13:53 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: servoPWM.c,v $
 * Revision 1.9  2010/10/06 12:13:53  clivewebster
 * Only set driver class if there are no errors
 *
 * Revision 1.8  2010/07/01 23:54:29  clivewebster
 * pin_make_output now specifies the initial output value
 *
 * Revision 1.7  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.6  2010/02/06 13:26:04  clivewebster
 * Allow setServoConfig to change the settings for a servo
 *
 * Revision 1.5  2009/11/02 19:05:25  clivewebster
 * Added revision log
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * servoPWM.c
 *
 *  Control servos using PWM
 *
 *  Created on: 16-Mar-2009
 *      Author: Clive Webster
 */
#include "../iopin.h"
#include "../timer.h"
#include "../core.h"
#include "../servos.h"

// Callback - for when the speed has been set
static void setSpeed(__ACTUATOR *actuator, DRIVE_SPEED speed){
	SERVO* servo = (SERVO*)actuator;
	const TimerCompare* channel = servo->channel;

	// Interpolate the values
	uint16_t delay = interpolateU(speed, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX, servo->min_ticks , servo->max_ticks);
//	servo->delay = delay;

	// Change the duty cycle
	compareSetThreshold(channel,delay);
}

static void setConnected(__ACTUATOR *actuator, boolean connected){
	SERVO* servo = (SERVO*)actuator;
	const TimerCompare* channel = servo->channel;

	// Turn on/off the pin
	compareSetOutputMode(channel, (connected) ? CHANNEL_MODE_NON_INVERTING : CHANNEL_MODE_DISCONNECT);
}

// Define the class
static const __ACTUATOR_DRIVER_CLASS PROGMEM c_servoPWM = MAKE_ACTUATOR_DRIVER_CLASS(&setSpeed,&setConnected);


// Pass the list of servos, the list should be in PROGMEM space to save Flash RAM
// The specified Timer must implement timer compare interrupts and, if so, it will
// ise the timer compare channel A (if there is more than one)
void servoPWMInit(SERVO_DRIVER* driver){
	// Make sure each servo is set as an output
	for(int i= driver->num_servos-1;i>=0;i--){
		SERVO* servo = (SERVO*)pgm_read_word(&driver->servos[i]);

		// Connect the servo to this driver
		servo->driver = driver;

		const TimerCompare* channel = compareFromIOPin(servo->pin);
		servo->channel = channel;
		const Timer* timer = compareGetTimer(channel);

		// Connect to driver
		servo->actuator.sclass = &c_servoPWM;

		// Make sure its an output pin, low
		pin_make_output(servo->pin, FALSE);

		// Get the value of TOP. Assumes mode = ICR
		const PORT icr = pgm_read_word(&timer->pgm_icr);
		servo->top = _SFR_MEM16(icr);


		servoSetConfig(servo, servo->center_us, servo->range_us);

		// Start off braking
		act_setSpeed(servo,DRIVE_SPEED_BRAKE);

		// Indicate the servo is connected
		act_setConnected(servo,TRUE);
	}
}

