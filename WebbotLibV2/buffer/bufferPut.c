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
 *        File: bufferPut.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../buffer.h"

// add a byte to the end of the buffer
boolean	bufferPut(cBuffer* buffer, uint8_t data){
	boolean rtn=FALSE; // failure

	// begin critical section
	CRITICAL_SECTION{

		// make sure the buffer has room
		if(buffer->datalength < buffer->size)
		{
			// save data byte at end of buffer
			buffer->dataptr[(buffer->dataindex + buffer->datalength) % buffer->size] = data;
			// increment the length
			buffer->datalength++;
			// return success
			rtn = TRUE;
		}
	}	// end critical section
	return rtn;
}

