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
 *        File: DroneCellPowerOn.c
 *  Created on: 1 Apr 2011
 *      Author: Clive Webster
 */
#include "../DroneCell.h"
#include "../../core.h"

/*	===========================================================================
 *
 * 					The power up sequence for a DroneCell
 *
	=========================================================================== */
//#define AVRSTUDIO




/*	===========================================================================
 *  Is there a SIM card present?
 *
 *  If the 'SIM Detect' pin is being used then read it
 *  Otherwise just assume the SIM is there
 *
 */
static boolean simPresent(const DRONECELL* dc){
	boolean rtn = TRUE;
	if(dc->simPin && pin_is_high(dc->simPin) ){
		rtn = FALSE;
	}
	return rtn;
}


/*	===========================================================================
 *
 *  This is the scheduler call back routine to perform the next step in the
 *  power up process.
 *
 */
static void _droneCellPowerOnProcess(SchedulerData data, TICK_COUNT lastTime, TICK_COUNT overflow){
	DRONECELL* dc = data;							// Which DroneCell are we powering up ?
	TICK_COUNT delay;

nextstep:
	dc->retries = 0;

retry:
	delay = 0;										// Default is that next step happens immediately
	__uartFlushReceiveBuffer(dc->uart);				// Flush out any incoming stuff during power up

	uint8_t response = DC_RESPONSE_OK;				// Default response for this step is 'OK'

#ifdef DEBUG_DRONECELL
PRINTF(DEBUG_DRONECELL,"POWER ON %u\n",dc->step);
#endif

	// Execute the correct code for the current step
	switch(dc->step){
	case 0:
		// See if its already powered on
		if(dc->forceReboot==FALSE && dc->presentPin && pin_is_high(dc->presentPin)){
			// Its already on
			dc->step = 3;
			goto retry;
		}
		// Set up the initial state of the power pin
		pin_make_output(dc->resetPin, TRUE);		// set power pin HIGH
		delay = 500000;								// wait for half a second before next step
		break;
	case 1:
		pin_make_output(dc->resetPin, FALSE);		// set power pin LOW
		delay = 2500000;							// wait for 2.5 seconds before next step
		break;
	case 2:
		pin_make_output(dc->resetPin, TRUE);		// set power pin HIGH again
		delay = 1000000;							// wait for 1 secondS for stuff to settle
		break;
	case 3:
		// Any future requests to reboot will do it
		dc->forceReboot = TRUE;

		// Should now be powered up
		if(dc->presentPin && pin_is_low(dc->presentPin)){
			// but its not there
			response = DC_RESPONSE_ERROR;
			dc->step = 0;								// Retry powering up (stage 0)
		}else{
			// It has started up
			delay = 5000000;						// wait 5 second to settle
		}
		break;

	case 4:
		// Lets start talking to it - but ignore any responses
		// for now as they may have echo turned on and long response codes turned on
		// so we wont understand it anyway
		for(int j=0; j<3;j++){
			droneCellSendATcommand(dc, null);					// Establish auto baud rate
			droneCellSendATcommand(dc, PSTR("V0"));				// Turn on short code - dont save setting
			droneCellSendATcommand(dc, PSTR("E0"));				// Turn off echo - dont save  setting
		}
		break;

	case 5:
		//Send AT and make sure we get an OK response
		response = droneCellSendATcommand(dc, null);
		break;

	case 6:
		//check there is a SIM card plugged in
		if(!simPresent(dc)){
			droneCellRelease(dc,DC_ERROR_SIM_MISSING);		// No point continuing !
			goto done;											// process returns error DC_ERROR_SIM_MISSING
		}
		break;

	case 7:
		// Request International Mobile Subscriber Identity
		// Another way to make sure the SIM is ok - ie not blocked etc
		response = droneCellSendATcommand(dc, PSTR("+CIMI"));
		break;

	case 8:
		// Set the SMS message centre to what has been set up in Project Designer
		// Its never going to change whilst running so may as well
		// do it now
		response = droneCellSendATcommand(dc,dc->smsc);
		break;

	case 9:
		// SMS Text message format (non-PDU mode)
		response = droneCellSendATcommand(dc,PSTR("+CMGF=1"));
		break;

	case 10:
		// Set the SMS message parameters
		response = droneCellSendATcommand(dc,PSTR("+CSMP=17,167,0,240"));
		break;

	case 11:
		// Set the SMS message mode to text
		response = droneCellSendATcommand(dc,PSTR("+CNMI=1"));
		break;




	default:
		// The process has finished successfully
		dc->poweredOn = TRUE;								// Mark DroneCell as powered on
		droneCellRelease(dc,DC_ERROR_NONE);					// End of process,
		goto done;												// Process returns DC_ERROR_NONE ie okay
	}


	// We have just finished a step and there is more to come

	// If this step has failed then retry that step or abandon
	if(response != DC_RESPONSE_OK){
		if(dc->retries++ < 5){								// Have we retried a few times ?
			goto retry;										// Retry it immediately
		}
		// We have tried enough - so start again
		dc->step = 0;
		goto nextstep;
//		droneCellRelease(dc,DC_ERROR_STARTUP);				// Cant power on, so ...
//		goto done;												// ... exit the process with DC_ERROR_STARTUP
	}

	// Advance to next step
	dc->step++;

#ifdef AVRSTUDIO
	delay = 0;
#endif

	// Response so far is ok - Schedule next step
	if(delay){												// If we need a pause
		scheduleJob(&_droneCellPowerOnProcess,dc,			// then run me again after the delay
				lastTime,delay);
		return;											//       and thats it for now
	}														// else there is no pause
	goto nextstep;											//		so just run it now

done:
	#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"*** POWER ON %s ****\n",(dc->result == DC_OK) ? "OK" : "FAILED");
	#endif
	return;
}

/*	===========================================================================
 *
 * 	Initiate the power on sequence for one DroneCell
 *
*/
void droneCellPowerOn(DRONECELL* dc){
	if(! dc->poweredOn){										// If not already powered on then
		_droneCellStartProcess(dc, &_droneCellPowerOnProcess,   //   wait for any existing process to finish
				DC_POWER_UP_TRY);								//   and then run this one
	}
}

