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
 *        File: marqueeSendByte.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../segled.h"
#include "../scheduler.h"
#include "../clock.h"

// For malloc
#include <stdlib.h>

static void marqueeUpdate(SchedulerData data, TICK_COUNT lastTime, TICK_COUNT overflow){
	MARQUEE* m = (MARQUEE*)data;

	char* readPos = m->txt + m->readPos;
	char  first=*readPos;
	boolean blink = m->blink;

	TICK_COUNT delay = (first=='\0') ? m->delayEnd : m->delayChar;
	TICK_COUNT delayDiv4 = delay >> 2;

	if(blink==FALSE){
		// we are not currently bliking. Check if we should start.
		if(first!='\0'){
			char* pos = readPos;
			char prev = readPos[-1];
			blink = TRUE;
			for(size_t led = 0; led < m->num_leds; led++){
				char ch = *pos++;
				// If we've hit end of line or a different char then no blink
				if(ch=='\0' || ch!=prev){
					blink=FALSE;
					break;
				}
				prev = ch;
			}
		}
	}else{
		// We are already blinking, so turn it off
		blink=FALSE;
	}

	// Set it for next time
	if(blink != m->blink){
		m->blink = blink;
		delay = (blink) ? delayDiv4 : delay - delayDiv4;
	}

	// Write chars to leds
	for(size_t l = 0; l < m->num_leds; l++){
		SEGLED* led = (SEGLED*)pgm_read_word(&m->leds[l]);
		char ch = (blink) ? '\0' : *readPos;
		if(ch!='\0'){
			readPos++;
		}else{
			ch = ' ';
		}
		segled_put_char(led,ch);
	}

	if(first=='\0'){
		// line is finished
		m->readPos = 0;
		if(m->delayEnd==0 || m->txt[0]=='\0'){
			m->active=FALSE;
		}else{
			m->blink = FALSE;
		}
	}else{
		// middle of line
		if(!blink){
			// Show character next time
			m->readPos += 1;
		}
	}

	if(m->active){
		scheduleJob(&marqueeUpdate,data,lastTime,delay);
	}
}


// Initialise a given marquee
static void marquee_init(MARQUEE* marquee){
	if(marquee->txt == null){
		marquee->txt = malloc(1 + marquee->txtSize);
		if(marquee->txt){
			for(int i=0; i<=marquee->txtSize;i++){
				marquee->txt[i] = '\0';
			}
			marquee->readPos = marquee->writePos = 0;
			marquee->active = FALSE;
			for(size_t i = 0; i < marquee->num_leds; i++){
				SEGLED* led = (SEGLED*)pgm_read_word(&marquee->leds[i]);
				segled_put_char(led,' ');
			}
		}
	}
}


uint8_t marqueeSendByte(MARQUEE* marquee, uint8_t byte){
	marquee_init(marquee);
	if(marquee->txt){

		if(byte=='\n'){
			// Start writing at the beginning of the line
			marquee->txt[marquee->writePos] = '\0';
			marquee->writePos = 0;
			CRITICAL_SECTION{
				if(!marquee->active){
					marquee->active = TRUE;
					marquee->blink = FALSE;
					marquee->readPos=0;
					scheduleJob(&marqueeUpdate,(SchedulerData)marquee,clockGetus(),marquee->delayChar);
				}
			}
		}else if(byte!='\r'){
			// Now put the character to the buffer
			if(marquee->writePos < marquee->txtSize){
				char* put = marquee->txt + marquee->writePos;
				CRITICAL_SECTION{
					*put++ = byte;
					*put = '\0';
					marquee->writePos += 1;
					marquee->readPos = 0;
				}
			}
		}
	}
	return byte;
}

