/*
 * $Id: tone.c,v 1.1 2010/10/09 17:19:32 clivewebster Exp $
 * Revision History
 * ================
 * $Log: tone.c,v $
 * Revision 1.1  2010/10/09 17:19:32  clivewebster
 * *** empty log message ***
 *
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
 *        File: tone.c
 *  Created on: 8 Oct 2010
 *      Author: Clive Webster
 */

#include "tone.h"

// Start playing a note
void tonePlay(TONE_PLAYER* player, uint16_t frequency, uint32_t durationMS ){
	const TONE_CLASS* class = player->clazz;
	if(class){
		void (*fn)(uint16_t frequency, uint32_t durationMS ) = (void (*)(uint16_t frequency, uint32_t durationMS ))pgm_read_word(&class->pgm_play);
		if(fn){
			fn(frequency,durationMS);
		}
	}
}

// Stop playing a tone
void toneStop(TONE_PLAYER* player){
	const TONE_CLASS* class = player->clazz;
	if(class){
		void (*fn)(void) = (void (*)(void))pgm_read_word(&class->pgm_stop);
		if(fn){
			fn();
		}
	}
}

