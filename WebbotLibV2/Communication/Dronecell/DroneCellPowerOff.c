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
 *        File: DroneCellPowerOff.c
 *  Created on: 1 Apr 2011
 *      Author: Clive Webster
 */
#include "../DroneCell.h"

static void _droneCellPowerOffProcess(SchedulerData data, TICK_COUNT lastTime, TICK_COUNT overflow){
	DRONECELL* dc = data;
	droneCellSendATcommand(dc, PSTR("+CPOWD=1"));			// Turn off the module
	dc->poweredOn = FALSE;

	// Process has finished with no errors
	droneCellRelease(dc,DC_ERROR_NONE);
}



//	Initiate the power off sequence
void droneCellPowerOff(DRONECELL* dc){
	if(dc->poweredOn){
		_droneCellStartProcess(dc, &_droneCellPowerOffProcess,DC_POWER_DOWN_TRY);
	}
}

