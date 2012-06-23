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
 *        File: timerCompare.c
 *  Created on: 16 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

extern const uint8_t PROGMEM _timer_compare_error;	// TIMER_COMPARE_CALLBACK_EXISTS


/* -------------------------------------------------------
//
//  Attach a callback function to a timer when the compare is met
//
------------------------------------------------------- */
void compareAttach(const TimerCompare* channel, TimerCompareCallback callback, uint16_t threshold, void* data ){

	const Timer* timer = compareGetTimer(channel);
	if(timerSupportsCompare(timer)){

		TimerDataCompare* td = compareGetData(channel);

		// Check if compare is already in use
		if(callback!=null && td->compare_callback!=null){
			setError(pgm_read_byte(&_timer_compare_error)); // pull in generated code
		}

		// Make sure the timer is set up and running
		timerSetPrescaler(timer,timerGetPrescaler(timer));

		// Disallow compare interrupts whilst changing
		__portMaskClear(&channel->pgm_intenable);

		// Set the compare threshold
		compareSetThreshold(channel, threshold);

		// Add the callback routine
		td->compare_callback = callback;
		td->compare_data = data;

		// Clear old interrupt pending before re-enabling interrupt
		compareClearInterruptPending(channel);

		// Allow compare interrupts - unless it is the null routine
		if(callback && callback != &nullTimerCompareCallback){
			__portMaskSet(&channel->pgm_intenable);
		}
	}else{
		setError(TIMER_COMPARE_NOT_SUPPORTED);
	}
}


/* -------------------------------------------------------
//
//  Detach any callback function from a timer
//
------------------------------------------------------- */
void compareDetach(const TimerCompare* channel)
{
	const Timer* timer = compareGetTimer(channel);
	if(timerSupportsCompare(timer)){

		// Disallow compare interrupts for this channel
		__portMaskClear(&channel->pgm_intenable);

		// Stop it from changing the output pin
		compareSetOutputMode(channel,CHANNEL_MODE_DISCONNECT);

		// remove routine
		compareGetData(channel)->compare_callback = null;
	}else{
		setError(TIMER_COMPARE_NOT_SUPPORTED);
	}
}

/* -------------------------------------------------------
//
//  A compare interrupt has been called for a given
//  timer. Do not call directly.
//
------------------------------------------------------- */
void __timer_compareService(const TimerCompare* channel){
	TimerDataCompare* td = compareGetData(channel);
	if(td->compare_callback){
		td->compare_callback(channel,td->compare_data);
	}
}

