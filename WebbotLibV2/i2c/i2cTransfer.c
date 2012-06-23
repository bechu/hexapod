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
 *        File: i2cTransfer.c
 *  Created on: 31 Dec 2010
 *      Author: Clive Webster
 */
#include "../i2cBus.h"
#include "../errors.h"

boolean i2cMasterTransfer(const I2C_DEVICE* device, size_t wlen, const uint8_t *wdata, size_t rlen, uint8_t * rdata){
	boolean ack = false;
	if(device){
		const I2C_ABSTRACT_BUS* i2c = device->bus;

		// Write the data
		ack = i2cStart(i2c,device->addr,TRUE);
		if(ack){
			// send the data
			while(ack && wlen--){
				ack &= i2cPut(i2c,*wdata++);
			}
		}

		// Read the response
		if(ack){
			ack = i2cStart(i2c,device->addr,FALSE);	// repeated start
		}
		if(ack){
			// Read the data
			while(rlen--){
				*rdata++ = i2cGet(i2c, (rlen) ? FALSE : TRUE);
			}
		}

		// Stop
		i2cStop(i2c);
	}
	return ack;
}

