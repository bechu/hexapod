/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2011 Clive Webster (webbot@webbot.org.uk)
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
 *        File: timerOverflow.c
 *  Created on: 16 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"
/* -------------------------------------------------------
//
//  Attach a callback function to a timer when it overflows
//
//  This function is called during an interrupt so should
//  be quick.
//
------------------------------------------------------- */
extern const uint8_t PROGMEM _timer_overflow_error;

void timerOverflowAttach(const Timer* timer, TimerCallback callback, void* user_data ){
	// Turn off interrupts whilst changing
	// in case an interrupt happens half way through
	CRITICAL_SECTION{
		if(callback!=null && timerGetData(timer)->overflow_callback != null){
			setError(pgm_read_byte(&_timer_overflow_error)); // pull in generated code
		}

		// Set callback
		TimerData* data = timerGetData(timer);
		data->overflow_callback = callback;
		data->overflow_data = user_data;

		// enable interrupt if there is callback
		if(callback!=null){
			// enable overflow interrupts
			__portMaskSet(&timer->pgm_overflowint);
		}else{
			// Disable overflow interrupts
			__portMaskClear(&timer->pgm_overflowint);
		}
	}
}

/* -------------------------------------------------------
//
//  An overflow interrupt has been called for a given
//  timer. Do not call directly.
//
------------------------------------------------------- */
void __timer_overflowService(const Timer* timer){
	TimerData* td = timerGetData(timer);
	if(td->overflow_callback){
		td->overflow_callback(timer,td->overflow_data);
	}
}



