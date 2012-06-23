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
 *        File: bufferDump.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../buffer.h"

// dump (discard) the first numbytes from the front of the buffer
void bufferDump(cBuffer* buffer, size_t numbytes){
	// begin critical section
	CRITICAL_SECTION{
		// dump numbytes from the front of the buffer
		// are we dumping less than the entire buffer?
		if(numbytes < buffer->datalength){
			// move index down by numbytes and decrement length by numbytes
			buffer->dataindex = (buffer->dataindex + numbytes) % buffer->size;
			buffer->datalength -= numbytes;
		}else{
			// flush the whole buffer
			buffer->datalength = 0;
		}
	}	// end critical section
}

