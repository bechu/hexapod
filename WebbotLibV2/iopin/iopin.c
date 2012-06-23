/*
 * $Id: iopin.c,v 1.7 2010/07/01 23:45:29 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: iopin.c,v $
 * Revision 1.7  2010/07/01 23:45:29  clivewebster
 * pin_make_output now specifies the initial output value
 *
 * Revision 1.6  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.5  2009/11/02 18:58:48  clivewebster
 * Added revision log
 *
 * ===========
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
 * iopin.c
 *
 *  Created on: 15-Mar-2009
 *      Author: Clive Webster
 */

#include "../iopin.h"

/******************************************************

	Set a port pin to high

******************************************************/
void pin_high(const IOPin* io){
	if(io){
		const IOPort* portDef = (const IOPort*)pgm_read_word(&io->port);
		PORT port = pgm_read_word(&portDef->port);
		PORT ddr = pgm_read_word(&portDef->ddr);
		PIN mask = pgm_read_byte(&io->pin);

		// make sure its an output
		if(_SFR_MEM8(ddr) & mask){
			CRITICAL_SECTION{
				// set the bit
				_SFR_MEM8(port) |= mask;
			}
			// Allow any output to be reflected on the input pin
			nop();
		}else{
			setError(PIN_SET_ON_INPUT);
		}
	}
 }

/******************************************************

	Set a port pin to low

******************************************************/
void pin_low(const IOPin* io){
	if(io){
		const IOPort* portDef = (const IOPort*)pgm_read_word(&io->port);
		PORT port = pgm_read_word(&portDef->port);
		PORT ddr = pgm_read_word(&portDef->ddr);
		PIN mask = pgm_read_byte(&io->pin);

		// make sure its an output
		if(_SFR_MEM8(ddr) & mask){
			CRITICAL_SECTION{
				// clear the bit
				_SFR_MEM8(port) &= ~mask;
			}
			// Allow any output to be reflected on the input pin
			nop();
		}else{
			setError(PIN_SET_ON_INPUT);
		}
	}
}




