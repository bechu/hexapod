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
 *
 *        File: main.c
 *  Created on: 24 Dec 2010
 *      Author: Clive Webster
 */

#include "core.h"
#include "clock.h"

static LOOP_COUNT loopCount;
static TICK_COUNT loopStart;
static TICK_COUNT delay;
extern void __initTimers(void);
extern void __clockInit(void);

//  This is the main loop for the application
int main(void){

	registerLED();

	// Initialise the clock
	__initTimers();
	__clockInit();
	sei();							// turn on interrupts (if not already on)

	// Initialise the hardware declared by the board
	sysInitHardware();
	// Initialise the hardware declared by the application
	appInitHardware();


	// Initialise the software
	loopStart = clockGetus();
	delay = appInitSoftware(loopStart);

	do{
		sei();							// turn on interrupts (if not already on)
		breathe();
		if(delay!=0){
			while(!clockHasElapsed(loopStart,delay)){
				// Allow interrupts to breathe
				breathe();
			}
		}
		loopStart = clockGetus();
		delay = appControl(++loopCount,loopStart);
	}while(1);
}

