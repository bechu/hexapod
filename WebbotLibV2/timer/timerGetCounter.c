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
 *        File: timerGetCounter.c
 *  Created on: 17 Jan 2011
 *      Author: Clive Webster
 */

#include "../timer.h"

uint16_t timerGetCounter(const Timer* timer){
	PORT counter = pgm_read_word(&timer->pgm_counter);
	if(timerIs16bit(timer)){
		return _SFR_MEM16(counter);
	}
	return _SFR_MEM8(counter);
}

