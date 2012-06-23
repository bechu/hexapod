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
 *        File: rprintfProgStr.c
 *  Created on: 26 Dec 2010
 *      Author: Clive Webster
 */
#include "../rprintf.h"

// prints a null-terminated string stored in program ROM
void rprintfProgStr(const char str[])
{
	if(str){
		// print a string stored in program memory
		register char c;

		// print the string until the null-terminator
		while((c = pgm_read_byte(str++)))
			rprintfChar(c);
	}
}

