
/*
 * $Id: _acceleration_dump.c,v 1.2 2010/06/14 18:50:17 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _acceleration_dump.c,v $
 * Revision 1.2  2010/06/14 18:50:17  clivewebster
 * Add copyright license info
 *
 * Revision 1.1  2010/01/26 21:07:07  clivewebster
 * *** empty log message ***
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
 * _acceleration_dump.c
 *
 *  Created on: 26-Jan-2010
 *      Author: Clive Webster
 */
#include "_acceleration_common.h"
#include "../../core.h"

void _accelerometerDumpTo(FILE*f, const ACCELEROMETER* device){
	PRINTF(f,"(x=%d mG, y=%d mG, z=%d mG)",
			device->x_axis_mG,
			device->y_axis_mG,
			device->z_axis_mG);
}
