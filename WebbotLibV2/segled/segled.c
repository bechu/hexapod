
/*
 * $Id: segled.c,v 1.7 2010/07/01 23:53:59 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: segled.c,v $
 * Revision 1.7  2010/07/01 23:53:59  clivewebster
 * pin_make_output now specifies the initial output value
 *
 * Revision 1.6  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.5  2010/02/21 19:56:29  clivewebster
 * Improve handling for packed fields
 *
 * Revision 1.4  2010/01/17 02:42:28  clivewebster
 * *** empty log message ***
 *
 * Revision 1.3  2009/12/29 12:38:33  clivewebster
 * Move short marquee calls into segledMarquee.c
 *
 * Revision 1.2  2009/12/28 19:20:56  clivewebster
 * *** empty log message ***
 *
 * Revision 1.1  2009/12/27 18:23:01  clivewebster
 * *** empty log message ***
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
 *
 * segled.c
 *
 *  Created on: 27-Dec-2009
 *      Author: Clive Webster
 *
 * 				---A---
 * 				|     |
 * 				F     B
 * 				|     |
 * 				---G---
 * 				|     |
 * 				E     C
 * 				|     |
 * 				---D--- H
 *
 *
 */
#include "../segled.h"


static void init(SEGLED* led){
	if(!led->initialised){
		for(uint8_t i=0; i<8;i++){
			const IOPin* pin = led->segment[i];
			pin_make_output(pin,FALSE);
		}
		led->initialised=TRUE;

	}
}

void segled_set(SEGLED* led, SEGLED_SEGMENT segment, boolean value){
	init(led);
	if(!led->activeHigh){
	 	value = (value) ? FALSE : TRUE;
	}
	const IOPin* pin = led->segment[segment];
	pin_set(pin,value);
}


uint8_t segled_put_char(SEGLED* led, uint8_t ch){
	init(led);
	uint8_t mask=segled8_get_mask(ch);	// get the mask
	if(mask & 0b00000001){
		// it uses the full stop
		if(!led->segment[7]){
			// no full stop - so just light segment D
			mask = 0b00010000;
		}
	}

	if(!led->activeHigh){
	 	mask ^= 0b11111111;
	}

	for(uint8_t i=0; i<8;i++){
		const IOPin* pin = led->segment[i];
		if(mask & 0b10000000){
			pin_high(pin);
		}else{
			pin_low(pin);
		}
		mask <<= 1;
	}
	return ch;
}

