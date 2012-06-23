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
 *        File: iopin_make_output.c
 *  Created on: 15 Jan 2011
 *      Author: Clive Webster
 */

#include "../iopin.h"


/******************************************************

	Set the pin as an output pin

******************************************************/
void pin_make_output(const IOPin* io, boolean val){
	if(io){
		const IOPort* portDef = (const IOPort*)pgm_read_word(&io->port);
		PORT ddr = pgm_read_word(&portDef->ddr);
		PORT port = pgm_read_word(&portDef->port);
		PIN mask = pgm_read_byte(&io->pin);

		CRITICAL_SECTION{
			if(val){
				_SFR_MEM8(port) |= mask;
			}else{
				_SFR_MEM8(port) &= ~ mask;
			}
			_SFR_MEM8(ddr) |= mask;	// make an output pin
		}
	}
}

