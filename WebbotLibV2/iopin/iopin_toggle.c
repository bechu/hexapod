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
 *        File: iopin_toggle.c
 *  Created on: 26 Dec 2010
 *      Author: Clive Webster
 */

#include "../iopin.h"

/******************************************************

	Toggle an output pin ie if low then make high,
	if high then make low
	eg
	pin_toggle(B2); // Toggle PortB pin 2

******************************************************/
void pin_toggle(const IOPin* io) {
	if(io){
		const IOPort* portDef = (const IOPort*)pgm_read_word(&io->port);
		PORT port = pgm_read_word(&portDef->port);
		PORT ddr = pgm_read_word(&portDef->ddr);
		PIN mask = pgm_read_byte(&io->pin);

		// make sure its an output
		if(_SFR_MEM8(ddr) & mask){
			CRITICAL_SECTION{
				// toggle the bit
				_SFR_MEM8(port) ^= mask;
			}
			// Allow any output to be reflected on the input pin
			nop();
		}else{
			setError(PIN_SET_ON_INPUT);
		}
	}
}

