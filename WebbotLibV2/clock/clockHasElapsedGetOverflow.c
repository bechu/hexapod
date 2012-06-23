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
 *        File: clockHasElapsedGetOverflow.c
 *  Created on: 24 Dec 2010
 *      Author: Clive Webster
 */

#include "../clock.h"

/*
   returns true if the specified number of microseconds
   has passed since the start time
   If true then set overflow to the number of microseconds
   that it is exceeded bt
*/
boolean clockHasElapsedGetOverflow(TICK_COUNT usStart, TICK_COUNT usWait, TICK_COUNT* overflow){
	boolean rtn=FALSE;
	TICK_COUNT now = clockGetus();
	TICK_COUNT elapsed = now;
	elapsed -= usStart;					// The actual delay that has happened

	if( elapsed >= usWait){
		// We have exceeded the delay
		if(overflow){
			TICK_COUNT ovr = elapsed - usWait;
			*overflow = ovr;
		}
		rtn = TRUE;
	}else{
		if(overflow){
			TICK_COUNT ovr = usWait - elapsed;
			*overflow = ovr;
		}
	}
	return rtn;
}

