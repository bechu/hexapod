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
 *        File: timerCapture.c
 *  Created on: 16 Jan 2011
 *      Author: Clive Webster
 */

#include "../timer.h"

extern const uint8_t PROGMEM _timer_capture_error;	// TIMER_CAPTURE_CALLBACK_EXISTS

void timerCaptureAttach(const Timer* timer, TimerCallback callback, void* user_data, boolean risingEdge ){
	TimerData* data = timerGetData(timer);
	if(data->capture_callback != null){
		setError(pgm_read_byte(&_timer_capture_error)); // pull in generated code
	}

	// Turn off interrupts whilst changing
	// in case an interrupt happens half way through
	CRITICAL_SECTION{

		// Set callback
		data->capture_callback = callback;
		data->capture_data = user_data;

		// Set the rising falling edge
		if(risingEdge){
			__portMaskSet(&timer->pgm_captureedge);
		}else{
			__portMaskClear(&timer->pgm_captureedge);
		}

		// Clear any pending capture interrupt flag
		timerCaptureClearInterruptPending(timer);

		// enable interrupt
		__portMaskSet(&timer->pgm_captureint);

	}
}

void timerCaptureDetach(const Timer* timer){
	TimerData* data = timerGetData(timer);

	// stop interrupt
	__portMaskClear(&timer->pgm_captureint);

	// Clear any pending capture interrupt flag
//	timerCaptureClearInterruptPending(timer);

	// Set callback
	data->capture_callback = null;
}

/* -------------------------------------------------------
//
//  A capture interrupt has been called for a given
//  timer. Do not call directly.
//
------------------------------------------------------- */
void __timer_captureService(const Timer* timer){
	TimerData* td = timerGetData(timer);

	if(td->capture_callback){
		td->capture_callback(timer,td->capture_data);
	}
}
