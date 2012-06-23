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
 *        File: timerGetPwmDeciHertz.c
 *  Created on: 17 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

uint32_t TimerGetPwmDeciHertz(const Timer* timer){
	uint32_t rtn = 0;
	if(timerIsInUse(timer)){
		TIMER_MODE mode = timerGetMode(timer);
		uint16_t prescale = timerGetPrescaler(timer);
		uint16_t icr;
		if(modeIsICR(mode)){
			PORT icrPort = pgm_read_word(&timer->pgm_icr);
			icr = _SFR_MEM16(icrPort);
		}else{
			icr = 0;
		}
		rtn =  __timerPWMGetDeciHertz(mode,prescale,icr);
	}
	return rtn;
}


