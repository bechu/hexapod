/*
 * $Id: GP2D120.c,v 1.8 2010/07/15 14:38:19 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2D120.c,v $
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

// Measures 4cm to 30cm
#include "GP2.h"

#define _ENTS_ 9
static const SHARP PROGMEM mytable [_ENTS_] = {
		MAKE_SHARP_ENTRY( 21, DISTANCE_MAX, DISTANCE_MAX),
		MAKE_SHARP_ENTRY( 30, 30, 23),
		MAKE_SHARP_ENTRY( 43, 22, 16),
		MAKE_SHARP_ENTRY( 61, 16, 12),
		MAKE_SHARP_ENTRY( 86, 12,  9),
		MAKE_SHARP_ENTRY(120,  9,  6),
		MAKE_SHARP_ENTRY(167,  6,  5),
		MAKE_SHARP_ENTRY(230,  5,  4),
		MAKE_SHARP_ENTRY(255, DISTANCE_MAX, DISTANCE_MAX)
};


// Read all the values and store into the device
static void __GP2D120_read(SENSOR* sensor){
	Sharp_GP2D120* device = (Sharp_GP2D120*)sensor;

//	uint16_t adc = a2dConvert8bit(device->adcPin);
//	device->distance.cm = 509.88 * pow(adc,-0.9154);

//	uint16_t adc = a2dReadMv(device->adcPin);
//	device->distance.cm = 7772.5 * pow(adc,-0.9154);

	device->distance.cm = _gp2_read(device->adcPin, &mytable[0], _ENTS_);
}

const DISTANCE_CLASS PROGMEM c_Sharp_GP2D120 = MAKE_DISTANCE_CLASS(null,&__GP2D120_read,0,0);

