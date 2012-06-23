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
 *        File: timerGetThreshold.c
 *  Created on: 3 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

uint16_t compareGetThreshold(const TimerCompare* channel){
	uint16_t rtn;

	const Timer* timer = compareGetTimer(channel);
	if(timerSupportsCompare(timer)){
		PORT port = (PORT)pgm_read_word(&channel->pgm_threshold);
		if(timerIs16bit(timer)){
			rtn = _SFR_MEM16(port); 					// set 16 bit word
		}else{
			rtn = _SFR_MEM8(port); 					// set low byte
		}
	}else{
		setError(TIMER_COMPARE_NOT_SUPPORTED);
		rtn = 0xff;
	}
	return rtn;
}

