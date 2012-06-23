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
 *        File: iopin_set.c
 *  Created on: 26 Dec 2010
 *      Author: Clive Webster
 */
#include "../iopin.h"
/******************************************************

	Set an output pin high or low
	eg
	pin_set(B2, TRUE); // To set PortB pin 2 high
******************************************************/
void pin_set(const IOPin* io, boolean val){
	if(val){
		pin_high(io);
	}else{
		pin_low(io);
	}
}


