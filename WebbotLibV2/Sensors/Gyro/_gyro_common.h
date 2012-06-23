/*
 * $Id: _gyro_common.h,v 1.5 2010/06/14 19:08:18 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _gyro_common.h,v $
 * Revision 1.5  2010/06/14 19:08:18  clivewebster
 * Add copyright license info
 *
 * Revision 1.4  2009/11/02 18:38:42  clivewebster
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
 * _gyro_common.h
 *
 *  Created on: 23-Sep-2009
 *      Author: Clive Webster
 *
 *  A gyro measures angular velocity in 'degrees_per_second' - a signed number
 *
 *
 */

#ifndef GYRO_H_
#define GYRO_H_
#include "../_sensor_common.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Define the data type used by all accelerators
// Changing this will mean changing gyroDump
#define GYRO_TYPE int16_t

// Define forward references
struct s_gyro;

// Define the api for the class
typedef struct /*PROGMEM*/ c_gyro{
	SENSOR_CLASS sensor;						// include parent class
} GYRO_CLASS;

#define MAKE_GYRO_CLASS(init,read,startup,delay) {MAKE_SENSOR_CLASS(init,read,startup,delay)}

// Define the data for each accelerometer
typedef struct s_gyro{
	SENSOR		sensor;							// Include fields from parent type
	GYRO_TYPE	x_axis_degrees_per_second;		// Last read value for the x axis
	GYRO_TYPE	y_axis_degrees_per_second; 		// Last read value for the y axis
	GYRO_TYPE	z_axis_degrees_per_second; 		// Last read value for the z axis
} GYRO;

#define MAKE_GYRO_SENSOR(class) {MAKE_SENSOR(&(class.sensor)),(GYRO_TYPE)0,(GYRO_TYPE)0,(GYRO_TYPE)0}

// Create macros to make the virtual methods easier to call
#define gyroRead(device) _sensorRead_(&device.gyro.sensor)
#define gyroInit(device) _sensorInit_(&device.gyro.sensor)

// Dump the value of a sensor to the rprintf destination
void _gyroDumpTo(FILE* f,const GYRO* device);
#define gyroDumpTo(f,device) _gyroDumpTo(f,&device.gyro);
#define gyroDump(device) _gyroDumpTo(stdout,&device.gyro);


#ifdef __cplusplus
}
class Gyro : public Sensor{
public:
	Gyro(GYRO* cstr) : Sensor(&cstr->sensor){
		m_cstr = cstr;
	}
	GYRO_TYPE getX(void) const{
		return m_cstr->x_axis_degrees_per_second;
	}
	GYRO_TYPE getY(void) const{
		return m_cstr->y_axis_degrees_per_second;
	}
	GYRO_TYPE getZ(void) const{
		return m_cstr->z_axis_degrees_per_second;
	}
//	virtual void dumpTo(Stream& str) const;
	virtual void dumpTo(FILE* str=stdout) const{
		_gyroDumpTo(str,m_cstr);
	}

private:
	GYRO* m_cstr;
};

#endif

#endif /* GYRO_H_ */

