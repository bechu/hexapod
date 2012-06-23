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
 *        File: StreamSigned.cpp
 *  Created on: 27 Apr 2011
 *      Author: Clive Webster
 */
#include "../libdefs.h"
#include <stdlib.h>
#include "Stream.h"

Stream& Stream::print(int8_t num, uint8_t base){
	return print( (int32_t) num, base);
}

Stream& Stream::print(int16_t num, uint8_t base){
	return print( (int32_t) num);
}

Stream& Stream::print(int32_t num, uint8_t base){
	char buf[8 * sizeof (uint32_t) + 1];	// enough for base 2
	if(base < 2) base = 2;
	return print(ltoa(num,buf,base));
}

Stream& Stream::print(int8_t num){
	return print( (int32_t) num, 10);
}

Stream& Stream::print(int16_t num){
	return print( (int32_t) num, 10);
}

Stream& Stream::print(int32_t num){
	return print( (int32_t) num, 10);
}
