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
 *        File: DS18B20.h
 *  Created on: 29 Jul 2011
 *      Author: Clive Webster
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "../_temperature_common.h"
#include "../../../oneWireBus.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

extern const TEMPERATURE_CLASS c_DS18B20;

typedef enum {
	DS18B20_RESOLUTION_9_BIT = 0,
	DS18B20_RESOLUTION_10_BIT,
	DS18B20_RESOLUTION_11_BIT,
	DS18B20_RESOLUTION_12_BIT	/* The power on default */
} DS18B20_RESOLUTION;

typedef struct s_1wire_ds18b20 {
	TEMPERATURE 	temperature;	// Include fields from parent type
	ONE_WIRE_DEVICE _wire1_;		// Include fields common to one wire device
	DS18B20_RESOLUTION resolution;	// The current resolution
} DS18B20;
#define MAKE_DS18B20_AT(a,b,c,d,e,f,crc, resolution) { \
	MAKE_TEMPERATURE_SENSOR(c_DS18B20), \
	MAKE_ONE_WIRE_DEVICE(DS18B20_ID,a,b,c,d,e,f,crc), \
	resolution \
}
#define MAKE_DS18B20(resolution) MAKE_DS18B20_AT(0,0,0,0,0,0,0, resolution)


#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class Ds18b20  : public Temperature, public OneWireDevice{
public:
	Ds18b20(DS18B20* cstr)
			: Temperature(&cstr->temperature), OneWireDevice(&cstr->_wire1_)
	{
//		m_cstr = cstr;
	}
private:
//	DS18B20* m_cstr;

};
#endif


#endif /* DS18B20_H_ */
