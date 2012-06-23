/*
 * $Id: GP2.h,v 1.3 2010/06/14 19:03:07 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: GP2.h,v $
 * Revision 1.3  2010/06/14 19:03:07  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/01/30 21:21:49  clivewebster
 * Added new sensors
 *
 * Revision 1.1  2010/01/27 18:39:54  clivewebster
 * Combine Sharp header files into one called GP2.h
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
 * GP2.h
 *
 *  Created on: 27-Jan-2010
 *      Author: Clive Webster
 */

#ifndef GP2_H_
#define GP2_H_

#include "../_distance_common.h"
#include "../../../a2d.h"
#include "../../../iopin.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

/*
 * GP2D12
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2D12;

typedef struct s_Sharp_GP2D12{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D12;

// Create the sensor
#define MAKE_Sharp_GP2D12(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D12), \
	adcPin }

/*
 * GP2D120
 */

// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2D120;

typedef struct s_Sharp_GP2D120{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D120;

// Create the sensor
#define MAKE_Sharp_GP2D120(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D120), \
	adcPin }

/*
 * GP2D15
 */

// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2D15;

typedef struct s_Sharp_GP2D15{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2D15;

// Create the sensor
#define MAKE_Sharp_GP2D15(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2D15), \
	adcPin }


/*
 * GP2Y0A02YK
 */

// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y0A02YK;

typedef struct s_Sharp_GP2Y0A02YK{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A02YK;

// Create the sensor
#define MAKE_Sharp_GP2Y0A02YK(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A02YK), \
	adcPin }

/*
 * GP2Y0A21YK
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y0A21YK;

typedef struct s_Sharp_GP2Y0A21YK{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A21YK;

// Create the sensor
#define MAKE_Sharp_GP2Y0A21YK(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A21YK), \
	adcPin }

/*
 * GP2Y0A02YK0F
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y0A02YK0F;

typedef struct s_Sharp_GP2Y0A02YK0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A02YK0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A02YK0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A02YK0F), \
	adcPin }


/*
 * GP2Y0A700K0F
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y0A700K0F;

typedef struct s_Sharp_GP2Y0A700K0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A700K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A700K0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A700K0F), \
	adcPin }

/*
 * GP2Y0A710K0F
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y0A710K0F;

typedef struct s_Sharp_GP2Y0A710K0F{
	DISTANCE distance;
	ADC_CHANNEL adcPin;
}Sharp_GP2Y0A710K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y0A710K0F(adcPin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y0A710K0F), \
	adcPin }


#if defined(BUILDING_LIBRARY)
// Stuff used by the library - dont use and dont edit...
typedef struct s_Sharp_Entry{
	uint8_t		  end_adc;
	DISTANCE_TYPE minDist;
	DISTANCE_TYPE maxDist;
} SHARP;
#define MAKE_SHARP_ENTRY(adc,min,max) { adc,min,max }

DISTANCE_TYPE _gp2_read(ADC_CHANNEL adc, const SHARP* table, uint8_t entries);
#endif

#ifdef __cplusplus
}
/* ===================== C++ Code ===============================================*/
class SharpIR : public Distance{
public:
	SharpIR(Sharp_GP2D12* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2D120* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2D15* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2Y0A02YK* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2Y0A02YK0F* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2Y0A21YK* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2Y0A700K0F* c) : Distance(&c->distance){}
	SharpIR(Sharp_GP2Y0A710K0F* c) : Distance(&c->distance){}
};

#endif

#endif /* GP2_H_ */
