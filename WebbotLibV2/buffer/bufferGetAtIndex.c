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
 *        File: bufferGetAtIndex.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */

#include "../buffer.h"

// get a byte at the specified index in the buffer (kind of like array access)
// ** note: this does not remove the byte that was read from the buffer
uint8_t	bufferGetAtIndex(const cBuffer* buffer, size_t index){
	uint8_t data;

	// begin critical section
	CRITICAL_SECTION{
		// return character at index in buffer
		data = (buffer) ? buffer->dataptr[(buffer->dataindex+index)%(buffer->size)] : 0;
		// end critical section
	}
	return data;
}

