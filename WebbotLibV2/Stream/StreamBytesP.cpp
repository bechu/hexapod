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
 *        File: StreamBytesP.cpp
 *  Created on: 27 Apr 2011
 *      Author: Clive Webster
 */

#include "../libdefs.h"
#include "Stream.h"

// Send a block of bytes from memory
size_t Stream::write_P(const void *src, size_t len){
	const uint8_t* bytes = (const uint8_t*)src;
	size_t rtn = 0;
	while(len--){
		if(write( pgm_read_byte(bytes++) ) == EOF){
			break;
		}
		rtn++;
	}
	return rtn;
}
