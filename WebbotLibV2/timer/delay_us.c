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
 *        File: delay_us.c
 *  Created on: 1 Jan 2011
 *      Author: Clive Webster
 */

#include "../timer.h"
#include "../clock.h"

/*
	Delay for a given number of microseconds
*/
void delay_us(uint32_t __us){
	if(__us > 100){
		clockWaitus(__us);
	}else{
		uint32_t cycles = (__us * cpu_speed_div_1000)/1000U;
		delay_cycles(cycles);
	}
}


