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
 *        File: iopin_is_output.c
 *  Created on: 26 Dec 2010
 *      Author: Clive Webster
 */
#include "../iopin.h"

/******************************************************

	Is the pin an output pin
	return TRUE if it is high or FALSE if low


******************************************************/

boolean pin_is_output(const IOPin* io){
	if(io){
		const IOPort* portDef = (const IOPort*)pgm_read_word(&io->port);
		PORT ddr = pgm_read_word(&portDef->ddr);
		PIN mask = pgm_read_byte(&io->pin);

		return (_SFR_MEM8(ddr) & mask) ? TRUE : FALSE;
	}
	return FALSE;
}


