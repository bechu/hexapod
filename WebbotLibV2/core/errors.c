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
 *        File: errors.c
 *  Created on: 24 Dec 2010
 *      Author: Clive Webster
 */

#include "../errors.h"
#include "../scheduler.h"
#include "../led.h"
#include "../clock.h"
#include "../core.h"
#include <stdlib.h>

ERROR __error; // All zeros so exclude -  = {0,0,FALSE,null};

ERROR_CODE getError(void){
	 return __error.errorCode;
}

/*
static void __error_flash(void * __error, TICK_COUNT lastTime, TICK_COUNT overflow){
	ERROR* err = (ERROR*) __error;

	if(err->remaining==0){
		err->remaining = ABS(err->errorCode);
	}

	TICK_COUNT delay = (err->errorCode <0 ) ? (TICK_COUNT)250000UL : (TICK_COUNT)500000UL;
	if(err->phase){
		// turn led off
		statusLED_off();
		err->remaining--;
		if(err->remaining==0){
			delay = (TICK_COUNT)2000000UL;
		}

		err->phase=FALSE;
	}else{
		// turn led on
		statusLED_on();
		err->phase=TRUE;

	}
	// queue the next event
	scheduleJob(&__error_flash, __error, lastTime, delay);

}
*/

void setError(ERROR_CODE err){
	if(__error.errorCode==0){
		__error.errorCode = err;
		// Start to flash the error code if we can
		if(statusLED.pin != null/* && g_heartbeat != null*/){
			statusLED_off();		// Start with it off
//			scheduleJob(&__error_flash, &__error, clockGetus(),0);
		}

		// log to stderr
		if(stderr){
			if(err < 0){
				PRINTF(stderr,"WebbotLib");
				err = -err;
			}else{
				PRINTF(stderr,"User");
			}
			PRINTF(stderr," Error:%d\r\n",err);
		}

		/*
		// log to any rprintf writer
		Writer writer = __error.output;
		if(writer){
			Writer old = rprintfInit(writer);
			if(err < 0 ){
				err = -err;
				rprintfProgStrM("WebbotLib");
			}else{
				rprintfProgStrM("User");
			}
			rprintfProgStrM(" Error:");
			// Put err as a number

			uint8_t div_val = 100;
			uint8_t u_val = err;

			// Strip leading zeroes
			while (div_val > 1 && div_val > u_val) div_val /= 10;
			do{
				div_t answer = div(u_val, div_val);
				rprintfChar(answer.quot + '0');
				u_val = answer.rem;
				div_val /= 10;
			} while (div_val);

			rprintfCRLF();
			rprintfInit(old);
		}
		*/
	}
}

void setErrorLog(Writer log){
	__error.output = log;
}


