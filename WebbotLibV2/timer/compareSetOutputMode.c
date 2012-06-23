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
 *        File: compareSetOutputMode.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../timer.h"

void compareSetOutputMode(const TimerCompare* channel, CHANNEL_MODE mode){
	PORT port = pgm_read_word(&channel->pgm_com.port);
	PIN  bit = pgm_read_byte(&channel->pgm_com.mask);
	mode <<= bit;

	PIN mask = BV(bit);	// occupies the next bit up as well
	mask |= (mask<<1);

	_SFR_MEM8(port) = (_SFR_MEM8(port) & ~mask) | (mode & mask);
}

