/*
 * $Id: _imu_common.h,v 1.1 2010/07/02 00:43:34 clivewebster Exp $
 * Revision History
 * ================
 * $Log: _imu_common.h,v $
 * Revision 1.1  2010/07/02 00:43:34  clivewebster
 * Added
 *
 * ================
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
 *        File: _imu_common.h
 *  Created on: 24 Jun 2010
 *      Author: Clive Webster
 */

#ifndef _IMU_COMMON_H_
#define _IMU_COMMON_H_

#include "../Acceleration/_acceleration_common.h"
#include "../Gyro/_gyro_common.h"
#include "../Compass/_compass_common.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Define forward references
struct s_imu;

// Define the api for the class
typedef struct /*PROGMEM*/ c_imu{
	SENSOR_CLASS sensor;						// include parent class
} IMU_CLASS;

#define MAKE_IMU_CLASS(init,read,startup,delay) {MAKE_SENSOR_CLASS(init,read,startup,delay)}

// Define the data for each imu sensor
typedef struct s_imu{
	SENSOR			sensor;							// Include fields from parent type
	ACCEL_TYPE  	x_axis_mG;
	ACCEL_TYPE 		y_axis_mG;
	ACCEL_TYPE 		z_axis_mG;
	GYRO_TYPE	x_axis_degrees_per_second;		// Last read value for the x axis
	GYRO_TYPE	y_axis_degrees_per_second; 		// Last read value for the y axis
	GYRO_TYPE	z_axis_degrees_per_second; 		// Last read value for the z axis
	COMPASS_TYPE	bearingDegrees;				// Last read value for the bearing, unsigned
	COMPASS_TYPE	rollDegrees;				// Last read value for the roll, signed
	COMPASS_TYPE	pitchDegrees;				// Last read value for the pitch, signed
} IMU;

#define MAKE_IMU_SENSOR(class) {MAKE_SENSOR(&(class.sensor)), \
		(ACCEL_TYPE)0,(ACCEL_TYPE)0,(ACCEL_TYPE)0, \
		(GYRO_TYPE)0,(GYRO_TYPE)0,(GYRO_TYPE)0, \
		(COMPASS_TYPE)0,(COMPASS_TYPE)0,(COMPASS_TYPE)0}

// Create macros to make the virtual methods easier to call
#define imuRead(device) _sensorRead_(&device.imu.sensor)
#define imuInit(device) _sensorInit_(&device.imu.sensor)

// Dump the value of a sensor to the rprintf destination
void _imuDumpTo(FILE* f,const IMU* device);
#define imuDumpTo(f,device) _imuDumpTo(f,&device.imu);
#define imuDump(device) _imuDumpTo(stdout,&device.imu);

#ifdef __cplusplus
}
class Imu : public Sensor{
public:
	Imu(IMU* cstr) : Sensor(&cstr->sensor){
		m_cstr = cstr;
	}
//	virtual void dumpTo(Stream& str) const;
	virtual void dumpTo(FILE* str=stdout) const{
		_imuDumpTo(str,m_cstr);
	}

	ACCEL_TYPE getAccelX(void) const{
		return m_cstr->x_axis_mG;
	}
	ACCEL_TYPE getAccelY(void) const{
		return m_cstr->y_axis_mG;
	}
	ACCEL_TYPE getAccelZ(void) const{
		return m_cstr->z_axis_mG;
	}
	GYRO_TYPE	getGyroX(void)const{
		return m_cstr->x_axis_degrees_per_second;
	}
	GYRO_TYPE	getGyroY(void)const{
		return m_cstr->y_axis_degrees_per_second;
	}
	GYRO_TYPE	getGyroZ(void)const{
		return m_cstr->z_axis_degrees_per_second;
	}
	COMPASS_TYPE getBearing(void) const{
		return m_cstr->bearingDegrees;
	}
	COMPASS_TYPE getRoll(void) const{
		return m_cstr->rollDegrees;
	}
	COMPASS_TYPE getPitch(void) const{
		return m_cstr->pitchDegrees;
	}


private:
	IMU* m_cstr;
};

#endif

#endif /* _IMU_COMMON_H_ */
