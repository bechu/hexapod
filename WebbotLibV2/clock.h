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
 *        File: clock.h
 *  Created on: 24 Dec 2010
 *      Author: Clive Webster
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "libdefs.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif


// A snaphot of the timer - it is of no direct practical use
// other than it can be turned into a time in uS
typedef struct s_timer_snapshot{
//	const Timer* timer;	// The timer
	TICK_COUNT whole;	// The overflow count
	uint16_t   part;	// The partial count
} TIMER_SNAPSHOT;

void clockGetSnapshot(TIMER_SNAPSHOT* snapshot);
TICK_COUNT clockSnapshotToTicks(const TIMER_SNAPSHOT* snapshot);

/* -------------------------------------------------------
//
//  Get the current heartbeat in microseconds
//
//  Note that this number will wrap around so a later reading
//  may give a smaller value
//  This happens every 0xffffffff or 429,4967,295 microseconds
//  ie every 429.5 seconds or every 7 minutes.
//  This means that the longest time difference you can sense
//  is about 7 minutes - this should not be a problem as you
//  will normally be using the heartbeat to measure durations
//  of less than one second.
------------------------------------------------------- */
TICK_COUNT clockGetus(void);

/*
   returns true if the specified number of microseconds
   has passed since the start time
*/
boolean clockHasElapsed(TICK_COUNT usStart, TICK_COUNT usWait);

/*
   returns true if the specified number of microseconds
   has passed since the start time
   If true then set overflow to the number of microseconds
   that it is exceeded bt
*/
boolean clockHasElapsedGetOverflow(TICK_COUNT usStart, TICK_COUNT usWait, TICK_COUNT* overflow);

/* waits (pauses) for the specified number of microseconds */
void clockWaitus(TICK_COUNT us);

/* waits (pauses) for the specified number of milliseconds */
void clockWaitms(TICK_COUNT ms);

#ifdef __cplusplus
}
// Create static methods for C++
class Clock {
public:
	static __inline__ boolean hasElapsed(TICK_COUNT usStart, TICK_COUNT usWait){
		return clockHasElapsed(usStart, usWait);
	}

	static __inline__  boolean hasElapsed(TICK_COUNT usStart, TICK_COUNT usWait, TICK_COUNT* overflow){
		return clockHasElapsedGetOverflow(usStart, usWait, overflow);
	}

	/* waits (pauses) for the specified number of microseconds */
	static  __inline__ void waitus(TICK_COUNT us){
		clockWaitus(us);
	}

	/* waits (pauses) for the specified number of milliseconds */
	static  __inline__ void waitms(TICK_COUNT ms){
		clockWaitms(ms);
	}

	static __inline__ TICK_COUNT getus(void){
		return clockGetus();
	}

	static __inline__ void getSnapshot(TIMER_SNAPSHOT* snapshot){
		clockGetSnapshot(snapshot);
	}

	static __inline__ TICK_COUNT snapshotToTicks(const TIMER_SNAPSHOT* snapshot){
		return clockSnapshotToTicks(snapshot);
	}

private:
	Clock() {};		// cannot create a Clock it is just a C++ wrapper
};

#endif

#endif /* CLOCK_H_ */
