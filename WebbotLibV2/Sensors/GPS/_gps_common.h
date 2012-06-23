/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
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
 *  Created on: 28-Apr-2011
 *      Author: Clive Webster
 *
 *  Defines the common interface for all gps sensors
 *
 */

#ifndef GPS_COMMON_H_
#define GPS_COMMON_H_
#include "../_sensor_common.h"
#include "gps.h"
#include <math.h>

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Define forward references
struct s_gps_common;

// Define the api for the class
typedef struct /*PROGMEM*/ c_gps{
	SENSOR_CLASS sensor;						// include parent class
} GPS_CLASS;

#define MAKE_GPS_CLASS(init,read,startup,delay) {MAKE_SENSOR_CLASS(init,read,startup,delay)}

// Define the data for each gps sensor
typedef struct s_gps_common{
	SENSOR	sensor;				// Include fields from parent type
	GPS		info;				// The values returned by the last read of the sensor
	GPS 	work;				// The values being changed under interrupts
} GPS_COMMON;

#define MAKE_GPS_SENSOR(class) {MAKE_SENSOR(&(class.sensor)),MAKE_GPS(), MAKE_GPS()}

// Create macros to make the virtual methods easier to call
#define gpsRead(device) _sensorRead_(&device.gps.sensor)
#define gpsInit(device) _sensorInit_(&device.gps.sensor)

// Dump the value of a sensor to the rprintf destination
void _gpsDumpTo(FILE* f,const GPS_COMMON* device);
#define gpsDumpTo(f,device) _gpsDumpTo(f,&device.gps);
#define gpsDump(device) _gpsDumpTo(stdout,&device.gps);

#if defined(BUILDING_LIBRARY)
void __gps_read(SENSOR* sensor);
#endif

#ifdef __cplusplus
}
class Gps : public Sensor{
public:
	Gps(GPS_COMMON* cstr) : Sensor(&cstr->sensor){
		m_cstr = cstr;
	}

//	virtual void dumpTo(Stream& str) const;
	virtual void dumpTo(FILE* str=stdout) const{
		_gpsDumpTo(str,m_cstr);
	}

	boolean isValid(void)const{
		return m_cstr->info.valid;
	}

	double getFixTime(void) const{
		return m_cstr->info.fixTime;
	}

	double getLongitudeDegrees(void) const{
		return m_cstr->info.longitude;
	}

	double getLongitudeRadians(void) const{
		return getLongitudeDegrees() * (M_PI/180.0);
	}

	double getLatitudeDegrees(void) const{
		return m_cstr->info.latitude;
	}

	double getLatitudeRadians(void) const{
		return getLatitudeDegrees() * (M_PI/180.0);
	}

	double getTrackDegrees(void) const{
		return m_cstr->info.track;
	}

	double getTrackRadians(void) const{
		return getTrackDegrees() * (M_PI/180.0);
	}

	double getSpeedKnots(void) const{
		return m_cstr->info.speed;
	}

	double getSpeedCMperSecond(void) const{
		return getSpeedKnots() * 51.4444444;
	}

	double getSpeedMPH(void) const{
		return getSpeedKnots() * 1.15077945;
	}

	int getNumSatellites(void) const{
		return m_cstr->info.numSatellites;
	}

	double getAltitudeMeter(void) const{
		return m_cstr->info.altitude;
	}

	boolean isUpdated(void)const{
		return (m_cstr->info.changes.all == 0) ? FALSE : TRUE;
	}

	boolean isFixTimeUpdated(void) const{
		return (m_cstr->info.changes.values.fixTime) ? TRUE : FALSE;
	}

	boolean isLongitudeUpdated(void) const{
		return (m_cstr->info.changes.values.longitude) ? TRUE : FALSE;
	}

	boolean isLatitudeUpdated(void) const{
		return (m_cstr->info.changes.values.latitude) ? TRUE : FALSE;
	}

	boolean isPositionUpdated(void) const{
		return (isLongitudeUpdated() || isLatitudeUpdated()) ? TRUE : FALSE;
	}

	boolean isSpeedUpdated(void) const{
		return (m_cstr->info.changes.values.speed) ? TRUE : FALSE;
	}

	boolean isSatellitesUpdated(void) const{
		return (m_cstr->info.changes.values.numSatellites) ? TRUE : FALSE;
	}

	boolean isAltitudeUpdated(void) const{
		return (m_cstr->info.changes.values.altitude) ? TRUE : FALSE;
	}

	boolean isTrackUpdated(void) const{
		return (m_cstr->info.changes.values.track) ? TRUE : FALSE;
	}


private:
	GPS_COMMON* m_cstr;
};
#endif

#endif /* GPS_COMMON_H_ */

