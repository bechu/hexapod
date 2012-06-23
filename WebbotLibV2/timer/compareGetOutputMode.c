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
 *        File: compareGetOutputMode.c
 *  Created on: 17 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

CHANNEL_MODE compareGetOutputMode(const TimerCompare* channel){
	PORT port = pgm_read_word(&channel->pgm_com.port);
	PIN  bit = pgm_read_byte(&channel->pgm_com.mask);

	PIN mask = BV(bit);	// occupies the next bit up as well
	mask |= (mask<<1);

	uint8_t val = _SFR_MEM8(port) & mask;
	val >>= bit;

	return (CHANNEL_MODE)val;
}
