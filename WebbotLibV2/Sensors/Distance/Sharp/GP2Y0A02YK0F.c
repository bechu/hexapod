
/*
 * $Id: GP2Y0A02YK0F.c,v 1.3 2010/07/15 14:38:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2Y0A02YK0F.c,v $
 * Revision 1.3  2010/07/15 14:38:19  clivewebster
 * Allow variable ADC reference voltage
 *
 * Revision 1.2  2010/06/14 19:03:07  clivewebster
 * Add copyright license info
 *
 * Revision 1.1  2010/01/30 21:21:49  clivewebster
 * Added new sensors
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
 * GP2Y0A02YK0F.c
 *
 *  Created on: 29-Jan-2010
 *      Author: Clive Webster
 */

// Measures 20cm to 150cm

#include "GP2.h"

#define _ENTS_ 13
static const SHARP PROGMEM myTable [_ENTS_] = {
		MAKE_SHARP_ENTRY( 23, DISTANCE_MAX, DISTANCE_MAX),
		MAKE_SHARP_ENTRY( 28,150,125),
		MAKE_SHARP_ENTRY( 34,120, 99),
		MAKE_SHARP_ENTRY( 40, 96, 82),
		MAKE_SHARP_ENTRY( 48, 80, 66),
		MAKE_SHARP_ENTRY( 57, 64, 54),
		MAKE_SHARP_ENTRY( 67, 53, 44),
		MAKE_SHARP_ENTRY( 79, 44, 36),
		MAKE_SHARP_ENTRY( 93, 36, 30),
		MAKE_SHARP_ENTRY(110, 30, 25),
		MAKE_SHARP_ENTRY(130, 24, 20),
		MAKE_SHARP_ENTRY(133, 20, 20),
		MAKE_SHARP_ENTRY(255, DISTANCE_MAX, DISTANCE_MAX)
};


// Read all the values and store into the device
static void __GP2Y0A02YK0F_read(SENSOR* sensor){
	Sharp_GP2Y0A02YK0F* device = (Sharp_GP2Y0A02YK0F*)sensor;

//	uint16_t adc = a2dConvert10bit(device->adcPin);
//	device->distance.cm = 34505.0 * pow(adc,-1.1908);

//	uint16_t adc = a2dReadMv(device->adcPin);
//	device->distance.cm = 228274.0 * pow(adc,-1.1908);

	device->distance.cm = _gp2_read(device->adcPin, &myTable[0], _ENTS_);

}

const DISTANCE_CLASS PROGMEM c_Sharp_GP2Y0A02YK0F = MAKE_DISTANCE_CLASS(null,&__GP2Y0A02YK0F_read,0,0);


