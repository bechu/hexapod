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
 *        File: timerSetMode.c
 *  Created on: 3 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

static const uint8_t PROGMEM __3bit_modes[]={0,1,255,255,2,3,255,255,255,255,255,5,255,255,255,7};
static const uint8_t PROGMEM __2bit_modes[]={0,1,255,255,2,3};

/* -------------------------------------------------------
//  Set the mode for a given timer
//  This will disconnect the output OCx pins on all
//  channels and will remove any callbacks on the channels
------------------------------------------------------- */
void timerSetMode(const Timer* timer, TIMER_MODE mode){
	if(mode == TIMER_MODE_NORMAL || timerGetMode(timer)!=mode){
		uint16_t abilities = pgm_read_word(&timer->pgm_modes);	// Get the bitmasks of modes availables

		if(BV(mode) & abilities){
			// It is capable of the mode

			// Save the new mode
			timerGetData(timer)->mode = mode;

			// Find the number of waveform generation bits
			uint8_t wgm=255;
			if(abilities == TIMER_ALL_MODES){
				wgm = mode;
			}else if(abilities == TIMER_3BIT_MODES){
				// Timer has the small subset of modes
				wgm = pgm_read_byte(&__3bit_modes[mode]);
			}else if(abilities == TIMER_2BIT_MODES){
				// Timer has the small subset of modes
				wgm = pgm_read_byte(&__2bit_modes[mode]);
			}


			if(wgm==255){
				// Mode is not supported
				setError(TIMER_MODE_NOT_SUPPORTED);
			}else{
				timerOff(timer);	// turn timer off whilst changing the mode

				const PORT_MASK* pm = &timer->pgm_wgm[0];
				for(int i=0; i<4;i++,pm++){
					PORT port= pgm_read_word(pm->port);

					if(port==0){
						break;
					}

					if(wgm & 1){
						__portMaskSet(pm);
					}else{
						__portMaskClear(pm);
					}
					wgm>>=1;
				}

				// re-initialise the timer :- set count to 0 and reset the prescaler to turn it back on
				PORT counter = pgm_read_word(&timer->pgm_counter);
				if(timerIs16bit(timer)){
					_SFR_MEM16(counter) = 0;
				}else{
					_SFR_MEM8(counter) = 0;
				}
				timerSetPrescaler(timer,timerGetPrescaler(timer));

			}
		}else{
			// Timer is not able to do this mode
			setError(TIMER_MODE_NOT_SUPPORTED);
		}
	}
}
