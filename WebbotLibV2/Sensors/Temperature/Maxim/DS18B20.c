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
 *        File: DS18B20.c
 *  Created on: 29 Jul 2011
 *      Author: Clive Webster
 */

#include "DS18B20.h"
#include "../../../oneWire/oneWireDefs.h"
#include "../../../core.h"

#define SCRATCHPAD_SIZE    	9

static void setResolution(DS18B20* ds, DS18B20_RESOLUTION res){
	ONE_WIRE_DEVICE* device = &ds->_wire1_;
	ONE_WIRE_BUS* bus = device->bus;
	res &= 3;

	if(bus){
		uint8_t scratch[SCRATCHPAD_SIZE];

		// Read the scratchpad
		oneWireMatchROM(device);
		oneWireWriteByte(bus,READSCRATCH);
		for(int i = 0; i < SCRATCHPAD_SIZE; i++){
			scratch[i] = oneWireReadByte(bus);
		}


		// Update the resolution bits
		uint8_t config = scratch[4];

		uint8_t oldRes = (config >> 5) & 3;
		if(oldRes != res){

			// The resolution has changed

			config &= 0x9F;
			config |= ( res << 5);
			scratch[4] = config;

			// Now write just the writable portion
			oneWireMatchROM(device);
			oneWireWriteByte(bus,WRITESCRATCH);
			for(int k = 2; k <= 4; k++){
				oneWireWriteByte(bus,scratch[k]);
			}
		}

		ds->resolution = res;
	}
}

static void init(SENSOR* sensor){
	DS18B20* device = (DS18B20*)sensor;
	if(device->_wire1_.found){
		setResolution(device, device->resolution);
	}
}

// Conversion time for each resolution in milliseconds
static const uint32_t PROGMEM conv[4] = { 94000,188000,375000,750000 };

static void read(SENSOR* sensor){
	DS18B20* ds = (DS18B20*)sensor;
	ONE_WIRE_DEVICE* device = &ds->_wire1_;
	ONE_WIRE_BUS* bus = device->bus;
	if(device->found){

		// Wait till done
		if(device->parasitic){
			// Its parasitic so keep applying power
			// Tell device to start converting
			oneWireMatchROM(device);
			oneWireWriteByte(bus,CONVERTTEMPERATURE);
			oneWireApplyPower(bus);
			uint32_t delay = pgm_read_dword(&conv[ds->resolution]);
	//		PRINTF(stdout,"Wait for %lu uS\r\n",delay);
			clockWaitus(delay);
		}else{
			// Its self powered so keep sampling till conversion done
			// Tell device to start converting
			oneWireMatchROM(device);
			oneWireWriteByte(bus,CONVERTTEMPERATURE);
			while(oneWireReadBit(bus)==0){
				breathe();
			}
		}

		// Read the scratchpad
		uint8_t scratch[SCRATCHPAD_SIZE];
		oneWireMatchROM(device);
		oneWireWriteByte(bus,READSCRATCH);
		uint8_t crc = 0;
		for(int i = 0; i < SCRATCHPAD_SIZE; i++){
			scratch[i] = oneWireReadByte(bus);
			crc = oneWireCRC(crc, scratch[i]);
		}

		if(crc!=0){
	//		PRINTF(stdout,"CRC Error\r\n");
		}else{
			// Set the actual resolution
			ds->resolution = (scratch[4] >> 5) & 3;

			uint8_t lsb = scratch[0];
		    uint8_t msb = scratch[1];

			// 'scratch' now has the temperature
		    int read_temp = (msb << 8) | lsb;
/*		    if(msb & 0x80){
		    	// its negative
		    	// TODO Dont think any of this is necessary
		    	read_temp = (read_temp ^ 0xFFFF) + 1;
		    	read_temp *= -1;
		    }
*/
	//	    PRINTF(stdout,"%X\r\n",read_temp);

		    switch(ds->resolution){
		    case DS18B20_RESOLUTION_9_BIT:
		    	read_temp &= 0xFFF8;
		    	break;
		    case DS18B20_RESOLUTION_10_BIT:
		    	read_temp &= 0xFFFC;
		    	break;
		    case DS18B20_RESOLUTION_11_BIT:
		    	read_temp &= 0xFFFE;
		    	break;
		    case DS18B20_RESOLUTION_12_BIT:
		    	break;
		    }

		    // read_temp is in 1/16ths of degrees celsius, ie divide it by 16
		    read_temp += 8;
		    read_temp >>= 4;

			ds->temperature.celsius = read_temp;
		}
	}
}

const TEMPERATURE_CLASS PROGMEM c_DS18B20 = MAKE_TEMPERATURE_CLASS(&init,&read, 0 ,0 );
