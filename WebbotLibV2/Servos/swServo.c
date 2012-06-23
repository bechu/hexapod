/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
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
 *	Common routines for all software servos
 *
 *        File: swServo.c
 *  Created on: 27 Dec 2010
 *      Author: Clive Webster
 */
#include "../servos.h"
#include "../core.h"

// Callback - for when the speed has been set
static void setSpeed(__ACTUATOR *actuator, DRIVE_SPEED speed){
	SERVO* servo = (SERVO*)actuator;

	// Interpolate the values
	uint16_t ticks=interpolateU(speed, DRIVE_SPEED_MIN, DRIVE_SPEED_MAX, servo->min_ticks , servo->max_ticks);
	if(ticks!=servo->delay){
		CRITICAL_SECTION{
			servo->delay = ticks;
		};
	}
}

// Define the class
const __ACTUATOR_DRIVER_CLASS PROGMEM c_Software_Servo = MAKE_ACTUATOR_DRIVER_CLASS(&setSpeed,null);

