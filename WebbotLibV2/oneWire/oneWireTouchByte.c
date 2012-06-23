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
 *        File: oneWireTouchByte.c
 *  Created on: 30 Jul 2011
 *      Author: Clive Webster
 */
#include "../oneWireBus.h"

//-----------------------------------------------------------------------------
// Write a 1-Wire data byte and return the sampled result.
//
uint8_t oneWireTouchByte(const ONE_WIRE_BUS* bus,uint8_t data){
	uint8_t result=0;

	for(int loop = 0; loop < 8; loop++){
		// shift the result to get it ready for the next bit
		result >>= 1;

		// If sending a '1' then read a bit else write a '0'
		if(data & 0x01){
			if(oneWireReadBit(bus)){
				result |= 0x80;
			}
		}else{
			oneWireWriteBit(bus,FALSE);
		}
		// shift the data byte for the next bit
		data >>= 1;
	}
	return result;
}

