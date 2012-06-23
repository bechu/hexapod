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
 *        File: memoryDump.c
 *  Created on: 24 May 2011
 *      Author: Clive Webster
 */

#include "../libdefs.h"
#include "../core.h"

void memoryDump(FILE* stream, const void* data, size_t off, size_t len){
    int x;
    int c;
    int line;
    const uint8_t * b= (const uint8_t *)data;

    PRINTF(stream,"ADDR|  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F | ASCII\r\n");
    for(int i=0;i<72;i++){
    	fputc('-',stream);
    }
	fputc('\r',stream);
	fputc('\n',stream);

	for(line = 0; line < ((len % 16 != 0) ? (len / 16) + 1 : (len / 16)); line++){
    	int line16 = line * 16;
    	PRINTF(stream,"%04x| ",line16);

        // put hex values
        for(x = 0; x < 16; x++){
            if(x + line16 < len){
                c = b[off + x + line16];
                PRINTF(stream,"%02x ",c);
            }else{
                PRINTF(stream,"   ");
            }
        }
        PRINTF(stream,"| ");

        // put ascii values
        for(x = 0; x < 16; x++){
            if(x + line16 < len){
                c = b[off + x + line16];
                if((c > 0x1f) && (c < 0x7f)){
                	// ascii
                	fputc(c,stream);
                }else{
                	// non ascii
                	fputc('.',stream);
                }
             }else{
             	fputc(' ',stream);
             }
        }
        fputc('\r',stream);
        fputc('\n',stream);
    }
    for(int i=0;i<72;i++){
    	fputc('-',stream);
    }
	fputc('\r',stream);
	fputc('\n',stream);
}
