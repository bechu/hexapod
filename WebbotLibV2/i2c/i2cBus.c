/*
 * $Id: i2cBus.c,v 1.3 2010/10/01 19:55:03 clivewebster Exp $
 * Revision History
 * ================
 * $Log: i2cBus.c,v $
 * Revision 1.3  2010/10/01 19:55:03  clivewebster
 * Setting the bit rate only applies to a hardware master
 *
 * Revision 1.2  2010/10/01 15:28:11  clivewebster
 * Refactored - merge master and slave into i2cBus.h
 *
 * Revision 1.1  2010/09/30 16:44:49  clivewebster
 * Added
 *
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
 *        File: i2cBus.c
 *  Created on: 24 Sep 2010
 *      Author: Clive Webster
 *
 *  Implement the common functions for an abstract i2c bus
 *
 */
#include "../i2cBus.h"
#include "../errors.h"


//--- Helper methods to redirect the call to the correct bus -----
void i2cAbstractBusInit(I2C_ABSTRACT_BUS* i2c){
	if(i2c && !i2c->initialised){
		const I2C_CLASS* class = i2c->sclass;

		// Put each device on this bus
		for(uint8_t i=0; i<i2c->numDevices;i++){
			I2C_DEVICE* device = (I2C_DEVICE*)pgm_read_word(&i2c->devices[i]);
			if(device){

				// Check for address conflicts
				for(uint8_t j=i+1; j<i2c->numDevices;j++){
					I2C_DEVICE* other = (I2C_DEVICE*)pgm_read_word(&i2c->devices[j]);
					if(other){
						if(device->addr == other->addr){
							setError(I2C_DUP_ADDR);
							return;
						}
					}
				}

				device->bus = i2c;
			}
		}

		// Call the initialisation method for the actual class
		void (*fn)(I2C_ABSTRACT_BUS*) = (void (*)(I2C_ABSTRACT_BUS*))pgm_read_word(&class->init);
		if(fn){
			fn(i2c);						// initialise the bus
		}
		i2c->initialised = TRUE;
	}
}

void 	i2cStop(const I2C_ABSTRACT_BUS* i2c){
	if(i2c){
		if(i2c->initialised){
			const I2C_CLASS* class = i2c->sclass;
			void (*fn)(const I2C_ABSTRACT_BUS*) =
					(void (*)(const I2C_ABSTRACT_BUS*))pgm_read_word(&class->stop);
			fn(i2c); 	// call the stop method
		}else{
			setError(I2C_BUS_NOT_INITIALIZED);
		}
	}
}

boolean  i2cStart(const I2C_ABSTRACT_BUS* i2c, uint8_t addr, boolean writeMode){
	boolean rtn = FALSE;
	if(i2c){
		if(i2c->initialised){
			const I2C_CLASS* class = i2c->sclass;
			boolean	 (*fn)(const I2C_ABSTRACT_BUS*,uint8_t,boolean) =
					(boolean (*)(const I2C_ABSTRACT_BUS*,uint8_t,boolean))pgm_read_word(&class->start);
			rtn = fn(i2c,addr,writeMode); 	// start
		}else{
			setError(I2C_BUS_NOT_INITIALIZED);
		}
	}
	return rtn;
}

void 	i2cAbstractSetBitrate(I2C_ABSTRACT_BUS* i2c,uint16_t bitrateKHz){
	if(i2c){

		const I2C_CLASS* class = i2c->sclass;

		// Call the  method for the actual class
		void (*fn)(const I2C_ABSTRACT_BUS*,uint16_t) = (void (*)(const I2C_ABSTRACT_BUS*,uint16_t))pgm_read_word(&class->speed);
		if(fn){
			fn(i2c,bitrateKHz);						// initialise the bus
		}
	}

}


