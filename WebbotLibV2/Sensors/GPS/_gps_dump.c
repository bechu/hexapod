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
 *        File: _gps_dump.c
 *  Created on: 28 Apr 2011
 *      Author: Clive Webster
 */
#include "_gps_common.h"
#include "../../core.h"

void _gpsDumpTo(FILE* f,const GPS_COMMON* device){
	const GPS* data = &device->info;
	PRINTF(f,"Valid:%c\n", (data->valid) ? 'Y' : 'N');
	PRINTF(f,"Satellites:%d\n",data->numSatellites);
	PRINTF(f,"\nLongitude(degrees):%f",data->longitude);
	PRINTF(f,"\nLatitude(degrees):%f",data->latitude);
	PRINTF(f,"\nTrack(degrees):%f",data->track);
	PRINTF(f,"\nAltitude(m):%f",data->altitude);
	PRINTF(f,"\nSpeed(knots):%f",data->speed);
	PRINTF(f,"\nTime:%f",data->fixTime);
	PRINTF(f,"\n");
}

