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
 *        File: HMC5883L.c
 *  Created on: 8 Aug 2011
 *      Author: Clive Webster
 */
#include "HMC5883L.h"

// Requires the floating point library
#include <math.h>
#include "../../../core.h"

#define MODE_CONTINUOUS 0
#define MODE_SINGLE 1
#define MODE_IDLE 2
#define MODE_SLEEP 3


static void _init(SENSOR* sensor){
	// Put into continuous measurement mode
	if(sensor){
		HMC5883L* device = (HMC5883L*)sensor;
		const I2C_DEVICE* i2c = &(device->i2cInfo);
		device->functional = FALSE;

		// Turn off top bit in CONFA
		uint8_t confA;
		if(i2cMasterReadRegisters(i2c,0,1,&confA)){
			confA &= 0x7f;
			i2cMasterWriteRegister(i2c, 0, confA);
		}

		if(i2cMasterWriteRegister(i2c, 2, MODE_SLEEP)){		// sleep mode
			delay_ms(10);
			if(i2cMasterWriteRegister(i2c, 2, MODE_SINGLE)){	// start a single conversion
				// keep looping until mode is set back to sleep mode with 2 second timeout
				// to indicate that the conversion is done
				uint8_t mode = MODE_SINGLE;
				for(uint8_t loop=0 ; loop<20 && mode==MODE_SINGLE; loop++){
					delay_ms(100);
					i2cMasterReadRegisters(i2c,2,1,&mode);		// read the mode register
				}


				// If its failed to do the single conversion there is a problem
				if(i2cMasterWriteRegister(i2c, 2, MODE_CONTINUOUS) && mode!=MODE_SINGLE){	// Continuous measurement mode
					device->functional = TRUE;
					device->compass.sensor.initialised = TRUE;
				}
			}
		}
	}
}

static void _read(SENSOR* sensor){
	HMC5883L* device = (HMC5883L*)sensor;
	const I2C_DEVICE* i2c = &(device->i2cInfo);
	uint8_t response[6];

	// Read the x,z,y registers (MSB first, then LSB) registers 3....8
	if(i2cMasterReadRegisters(i2c,3,sizeof(response),response)){
		int16_t x = (response[0] << 8) | response[1];
		int16_t z = (response[2] << 8) | response[3];
		int16_t y = (response[4] << 8) | response[5];
		device->rawX = x;
		device->rawY = y;
		device->rawZ = z;

		double heading = atan2((double)(x), (double)(-1 * y)) * (180.0 / M_PI);
		if(heading < 0)	heading += 360;
		device->compass.bearingDegrees = (COMPASS_TYPE)heading;

		heading = atan2((double)(x), (double)(-1 * z)) * (180.0 / M_PI);
		if(heading < 0)	heading += 360;
		device->compass.rollDegrees = (COMPASS_TYPE)heading;

		heading = atan2((double)(y), (double)(-1 * z)) * (180.0 / M_PI);
		if(heading < 0)	heading += 360;
		device->compass.pitchDegrees = (COMPASS_TYPE)heading;
	}
}

// Set the speed in bits 4,3,2 of configuration register A (register 00) (
static void _speed(HMC5883L* compass, uint8_t speed){
	if(compass){
		uint8_t reg;
		const I2C_DEVICE* i2c = &(compass->i2cInfo);
		if(i2cMasterReadRegisters(i2c,0x00,1,&reg)){
			reg &= 0xE3;	// mask out other bits
			reg |= (speed << 2);
			i2cMasterWriteRegister(i2c,0x00,reg);

			// Initialise
			_init(&compass->compass.sensor);
		}
	}
}

void HMC5883L_1_5Hz(HMC5883L* compass){
	_speed(compass,1);
}
void HMC5883L_3Hz(HMC5883L* compass){
	_speed(compass,2);
}
void HMC5883L_7_5Hz(HMC5883L* compass){
	_speed(compass,3);
}
void HMC5883L_15Hz(HMC5883L* compass){
	_speed(compass,4);
}
void HMC5883L_30Hz(HMC5883L* compass){
	_speed(compass,5);
}
void HMC5883L_75Hz(HMC5883L* compass){
	_speed(compass,6);
}


// Requires a 100ms delay before being read for the first time
// The fastest speed is 75Hz ie every 13.33ms
const COMPASS_CLASS c_HMC5883L = MAKE_COMPASS_CLASS(&_init,&_read, 100 , 14 );

