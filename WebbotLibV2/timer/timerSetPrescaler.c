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
 *        File: timerSetPrescaler.c
 *  Created on: 3 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

void timerSetPrescaler(const Timer* timer, uint16_t prescaler){
	PORT pre = pgm_read_word(&timer->pgm_prescaler);

	CRITICAL_SECTION{
		int inx = timerGetPrescalerIndex(prescaler,timerIsRTC(timer));
		_SFR_MEM8(pre) &= ~ TIMER_PRESCALE_MASK;
		if(inx!=-1){
			timerGetData(timer)->prescale_value = prescaler;
			_SFR_MEM8(pre) |= inx;
		}else{
			setError(TIMER_PRESCALER_UNSUPPORTED);
		}
	}

}
