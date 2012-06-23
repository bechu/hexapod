/*
 * $Id: GP2Y0A02YK.c,v 1.8 2010/07/15 14:38:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2Y0A02YK.c,v $
 * Revision 1.8  2010/07/15 14:38:19  clivewebster
 * Allow variable ADC reference voltage
 *
 * Revision 1.7  2010/06/14 19:03:07  clivewebster
 * Add copyright license info
 *
 * Revision 1.6  2010/01/27 18:39:54  clivewebster
 * Combine Sharp header files into one called GP2.h
 *
 * Revision 1.5  2010/01/24 17:18:14  clivewebster
 * Use ADC channels in constructor rather than IOPins
 *
 * Revision 1.4  2009/12/11 17:17:56  clivewebster
 * Fixed #include for Unix
 *
 * Revision 1.3  2009/11/02 18:32:14  clivewebster
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
 */

// Measures 20cm to 150cm

#include "GP2.h"

#define _ENTS_ 14
static const SHARP PROGMEM myTable [_ENTS_] = {
		MAKE_SHARP_ENTRY( 17, DISTANCE_MAX, DISTANCE_MAX),
		MAKE_SHARP_ENTRY( 21,145,127),
		MAKE_SHARP_ENTRY( 26,122,105),
		MAKE_SHARP_ENTRY( 32,101, 87),
		MAKE_SHARP_ENTRY( 39, 85, 73),
		MAKE_SHARP_ENTRY( 48, 71, 61),
		MAKE_SHARP_ENTRY( 58, 60, 51),
		MAKE_SHARP_ENTRY( 70, 51, 32),
		MAKE_SHARP_ENTRY( 85, 43, 37),
		MAKE_SHARP_ENTRY(103, 36, 31),
		MAKE_SHARP_ENTRY(125, 31, 26),
		MAKE_SHARP_ENTRY(151, 26, 22),
		MAKE_SHARP_ENTRY(172, 22, 20),
		MAKE_SHARP_ENTRY(255, DISTANCE_MAX, DISTANCE_MAX)
};

// Read all the values and store into the device
static void __GP2Y0A02YK_read(SENSOR* sensor){
	Sharp_GP2Y0A02YK* device = (Sharp_GP2Y0A02YK*)sensor;

//	uint16_t adc = a2dConvert8bit(device->adcPin);
//	device->distance.cm = 1904.5 * pow(adc,-0.89);

//	uint16_t adc = a2dReadMv(device->adcPin);
//	device->distance.cm = 26918.0 * pow(adc,-0.89);

	device->distance.cm = _gp2_read(device->adcPin, &myTable[0], _ENTS_);

}

const DISTANCE_CLASS PROGMEM c_Sharp_GP2Y0A02YK = MAKE_DISTANCE_CLASS(null,&__GP2Y0A02YK_read,0,0);
