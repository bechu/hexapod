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
 *        File: delay_cycles.c
 *  Created on: 1 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"
/*
	Delay for a given number of processor cycles
*/
void delay_cycles(uint32_t __cycles){
	if(__cycles <= (3*256U)){
		_delay_loop_1((uint8_t) (__cycles / 3) );
		return;
	}

	uint32_t cyc = __cycles / 4;
	while(cyc != 0 ){
	   uint16_t actual = (cyc <= 65535U) ? cyc : 65535U;
	   _delay_loop_2(actual);
	   cyc -= actual;
	}
}

