/*
 * $Id: rprintfs.c,v 1.6 2010/06/15 00:48:59 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: rprintfs.c,v $
 * Revision 1.6  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.5  2010/01/24 12:13:32  clivewebster
 * *** empty log message ***
 *
 * Revision 1.4  2009/11/02 19:02:47  clivewebster
 * Added revision log
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
 * rprintfs.c
 *
 *  Created on: 15-Mar-2009
 *      Author: Clive Webster
 */
#include "../rprintf.h"
#include "../libdefs.h"
#include <stdarg.h>
#include <stdio.h>

static int rprintfOut(char c, FILE* f){
	return rprintfChar(c);
}
static int rprintfIn(FILE* f){
	return EOF;
}
static FILE io = FDEV_SETUP_STREAM(&rprintfOut,&rprintfIn,_FDEV_SETUP_WRITE);

int rprintfRamRom(unsigned char stringInRom, const char *format, ...){
	int rtn;
	va_list args;
	va_start (args, format);
	if(stringInRom == STRING_IN_ROM){
		rtn=vfprintf_P(&io, format, args);
	}else{
		rtn=vfprintf(&io, format, args);
	}
	va_end (args);
	return rtn;
}

