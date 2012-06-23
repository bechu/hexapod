/*
 * $Id: GP2D12.c,v 1.8 2010/07/15 14:38:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2D12.c,v $
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

// Measures 10cm to 80cm

#include "GP2.h"

#define _ENTS_ 12
static const SHARP PROGMEM myTable [_ENTS_] = {
		MAKE_SHARP_ENTRY( 17, DISTANCE_MAX, DISTANCE_MAX),
		MAKE_SHARP_ENTRY( 22, 77, 63),
		MAKE_SHARP_ENTRY( 28, 60, 50),
		MAKE_SHARP_ENTRY( 36, 48, 39),
		MAKE_SHARP_ENTRY( 45, 38, 31),
		MAKE_SHARP_ENTRY( 57, 30, 24),
		MAKE_SHARP_ENTRY( 71, 24, 20),
		MAKE_SHARP_ENTRY( 89, 19, 16),
		MAKE_SHARP_ENTRY(111, 15, 13),
		MAKE_SHARP_ENTRY(138, 12, 10),
		MAKE_SHARP_ENTRY(146, 10, 10),
		MAKE_SHARP_ENTRY(255, DISTANCE_MAX, DISTANCE_MAX)
};



// Read all the values and store into the device
static void __GP2D12_read(SENSOR* sensor){
	Sharp_GP2D12* device = (Sharp_GP2D12*)sensor;

//	uint16_t adc = a2dConvert8bit(device->adcPin);
//	device->distance.cm = 1384.4 * pow(adc,-0.9988);

//	uint16_t adc = a2dReadMv(device->adcPin);
//	device->distance.cm = 27048.0 * pow(adc,-0.9988);

	device->distance.cm = _gp2_read(device->adcPin, &myTable[0], _ENTS_);
}

const DISTANCE_CLASS PROGMEM c_Sharp_GP2D12 = MAKE_DISTANCE_CLASS(null,&__GP2D12_read,0,0);


