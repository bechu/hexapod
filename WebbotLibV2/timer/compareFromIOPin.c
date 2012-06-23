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
 *        File: compareFromIOPin.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../timer.h"
/* -------------------------------------------------------
//
//  Get the compare channel used by a given IOPin
//  May return null if this pin is not to do with a timer
//  or it does not exist on this system board
//  An IOPin may actually support more than one compare
// channel - in which case give priority to 16 bit timers
------------------------------------------------------- */
const TimerCompare* compareFromIOPin(const IOPin* pin){
	uint8_t t;
	uint8_t c;
	const TimerCompare* rtn=null;

	// Iterate over the timers
	for(t=0; t < NUMBER_OF_TIMERS; t++){
		const Timer * timer = &pgm_Timers[t];

		// Iterate over the compare units
		uint8_t numCompare = timerNumberOfCompareUnits(timer);
		for(c=0;c<numCompare;c++){
			const TimerCompare* channel = timerGetCompare(timer,c);
			const IOPin* thisPin = compareGetPin(channel);
			if(thisPin != null && thisPin==pin){
				rtn = channel;					// remember the channel
				if(timerIs16bit(timer)){
					return rtn;
				}
			}
		}
	}

	return rtn;
}

