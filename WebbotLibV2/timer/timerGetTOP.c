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
 *        File: timerGetTOP.c
 *  Created on: 31 Dec 2010
 *      Author: Clive Webster
 */
#include "../timer.h"

uint16_t timerGetTOP(const Timer* timer){
	uint16_t rtn = 0;
	if(timerIsInUse(timer)){
		switch(timerGetMode(timer)){
		case TIMER_MODE_NORMAL:
			rtn = 0xffffU;
			break;
		case TIMER_MODE_PWM8_PHASE_CORRECT:
		case TIMER_MODE_PWM8_FAST:
			rtn = 0xffU;
			break;
		case TIMER_MODE_PWM9_PHASE_CORRECT:
		case TIMER_MODE_PWM9_FAST:
			rtn = 0x1ffU;
			break;
		case TIMER_MODE_PWM10_PHASE_CORRECT:
		case TIMER_MODE_PWM10_FAST:
			rtn = 0x3ffU;
			break;

		// The following all use channel OCRnA	for top and fires compare match interrupts instead of overflows
		case TIMER_MODE_CTC_OCR:
		case TIMER_MODE_PWM_PHASE_FREQ_OCR:
		case TIMER_MODE_PWM_PHASE_CORRECT_OCR:
		case TIMER_MODE_PWM_FAST_OCR:
			{
				const TimerCompare* tc = timerGetCompare(timer,0); // uses channel A
				PORT port = (PORT)pgm_read_word(&tc->pgm_threshold);
				rtn = _SFR_MEM16(port); 					// set 16 bit word
			}
			break;

		// The following all use ICRn for top and fires ICFn interrupts instead of overflows
		case TIMER_MODE_PWM_PHASE_FREQ_ICR:
		case TIMER_MODE_PWM_PHASE_CORRECT_ICR:
		case TIMER_MODE_CTC_ICR:
		case TIMER_MODE_PWM_FAST_ICR:
			{
				PORT icr = pgm_read_word(&timer->pgm_icr);
				rtn = _SFR_MEM16(icr);
			}
			break;
		case TIMER_MODE_13_RESVD:
			break;
		}// end switch
	}
	if(!timerIs16bit(timer)){
		rtn &= 0xffu;
	}
	return rtn;
}

