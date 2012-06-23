/*
 * $Id: _voltage_common.h,v 1.4 2010/06/14 19:11:42 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _voltage_common.h,v $
 * Revision 1.4  2010/06/14 19:11:42  clivewebster
 * Add copyright license info
 *
 * Revision 1.3  2009/11/02 18:44:49  clivewebster
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
 *  Created on: 19-Sep-2009
 *      Author: Clive Webster
 *
 *  Defines the common interface for all voltage sensors
 *
 */

#ifndef VOLTAGE_H_
#define VOLTAGE_H_
#include "../_sensor_common.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Define the data type used by all sensors
// Changing this will mean changing voltageDump
#define VOLTAGE_TYPE int16_t

// Define forward references
struct s_voltage;

// Define the api for the class
typedef struct /*PROGMEM*/ c_voltage{
	SENSOR_CLASS sensor;						// include parent class
} VOLTAGE_CLASS;

#define MAKE_VOLTAGE_CLASS(init,read,startup,delay) {MAKE_SENSOR_CLASS(init,read,startup,delay)}

// Define the data for each voltage sensor
typedef struct s_voltage{
	SENSOR			sensor;						// Include fields from parent type
	VOLTAGE_TYPE	volts;						// Last read value
} VOLTAGE;

#define MAKE_VOLTAGE_SENSOR(class) {MAKE_SENSOR(&(class.sensor)),(VOLTAGE_TYPE)0}

// Create macros to make the virtual methods easier to call
#define voltageRead(device) _sensorRead_(&device.voltage.sensor)
#define voltageInit(device) _sensorInit_(&device.voltage.sensor)

// Dump the value of a sensor to the rprintf destination
void _voltageDumpTo(FILE* f,const VOLTAGE* device);
#define voltageDumpTo(f,device) _voltageDumpTo(f,&device.voltage);
#define voltageDump(device) _voltageDumpTo(stdout,&device.voltage);

#ifdef __cplusplus
}
class Voltage : public Sensor{
public:
	Voltage(VOLTAGE* cstr) : Sensor(&cstr->sensor){
		m_cstr = cstr;
	}
//	virtual void dumpTo(Stream& str) const;
	virtual void dumpTo(FILE* str=stdout) const{
		_voltageDumpTo(str,m_cstr);
	}

	VOLTAGE_TYPE getVolts(void)const {
		return m_cstr->volts;
	}
private:
	VOLTAGE* m_cstr;
};

#endif

#endif /* VOLTAGE_H_ */

