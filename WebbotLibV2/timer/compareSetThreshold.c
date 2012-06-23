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
 *        File: compareSetThreshold.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */

#include "../timer.h"

void compareSetThreshold(const TimerCompare* channel, uint16_t threshold){
	const Timer* timer = compareGetTimer(channel);
	if(timerSupportsCompare(timer)){
		PORT port = (PORT)pgm_read_word(&channel->pgm_threshold);
		if(timerIs16bit(timer)){
			_SFR_MEM16(port) = threshold; 					// set 16 bit word
		}else{
			if(threshold > 0xffU){
				threshold=0xffU;
				setError(TIMER_COMPARE_NOT_8_BIT);
			}
			_SFR_MEM8(port) = threshold; 				// set low byte
		}
	}else{
		setError(TIMER_COMPARE_NOT_SUPPORTED);
	}
}

