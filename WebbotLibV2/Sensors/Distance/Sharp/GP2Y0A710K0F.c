
/*
 * $Id: GP2Y0A710K0F.c,v 1.3 2010/07/15 14:38:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2Y0A710K0F.c,v $
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
 * GP2Y0A710K0F.c
 *
 *  Created on: 29-Jan-2010
 *      Author: Clive Webster
 */

// Measure 100cm to 550cm

#include "GP2.h"

#define _ENTS_ 17
static const SHARP PROGMEM myTable [_ENTS_] = {
		MAKE_SHARP_ENTRY( 28, DISTANCE_MAX, DISTANCE_MAX),
		MAKE_SHARP_ENTRY( 32,533,479),
		MAKE_SHARP_ENTRY( 36,463,421),
		MAKE_SHARP_ENTRY( 40,409,375),
		MAKE_SHARP_ENTRY( 45,365,330),
		MAKE_SHARP_ENTRY( 50,322,294),
		MAKE_SHARP_ENTRY( 56,288,260),
		MAKE_SHARP_ENTRY( 62,255,233),
		MAKE_SHARP_ENTRY( 69,229,207),
		MAKE_SHARP_ENTRY( 77,204,184),
		MAKE_SHARP_ENTRY( 85,181,165),
		MAKE_SHARP_ENTRY( 94,163,148),
		MAKE_SHARP_ENTRY(104,146,133),
		MAKE_SHARP_ENTRY(115,131,119),
		MAKE_SHARP_ENTRY(127,118,107),
		MAKE_SHARP_ENTRY(135,106,100),
		MAKE_SHARP_ENTRY(255, DISTANCE_MAX, DISTANCE_MAX)
};

// Read all the values and store into the device
static void __GP2Y0A710K0F_read(SENSOR* sensor){
	Sharp_GP2Y0A710K0F* device = (Sharp_GP2Y0A710K0F*)sensor;

//	uint16_t adc = a2dConvert10bit(device->adcPin);
//	device->distance.cm = 500000000.0 * pow(adc,-2.4723);

//	uint16_t adc = a2dReadMv(device->adcPin);
//	device->distance.cm = 25270743335.99 * pow(adc,-2.4723);

	device->distance.cm = _gp2_read(device->adcPin, &myTable[0], _ENTS_);

}

const DISTANCE_CLASS PROGMEM c_Sharp_GP2Y0A710K0F = MAKE_DISTANCE_CLASS(null,&__GP2Y0A710K0F_read,0,0);

