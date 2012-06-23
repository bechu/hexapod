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
 *
 *        File: i2cReadRegisters.c
 *  Created on: 31 Dec 2010
 *      Author: Clive Webster
 */

#include "../i2cBus.h"
#include "../errors.h"

boolean i2cMasterReadRegisters(const I2C_DEVICE* device, uint8_t startReg, size_t numBytes, uint8_t* response){
	return i2cMasterTransfer(device, 1, &startReg, numBytes, response);
}

