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
 *        File: DroneCellSMS.c
 *  Created on: 1 Apr 2011
 *      Author: Clive Webster
 */
#include "../DroneCell.h"
#include "../../rprintf.h"
#include "../../core.h"
#include "../../buffer.h"
#include <string.h>
#include <stdlib.h>


/*
 *  Prepare the device for sending an SMS message
 *
 *  This will return immediately and the return code
 *  indicates whether or not the user can write the SMS message
 *
 */
boolean droneCellSMSInit(DRONECELL* dc, const char callNumber[]) {

	// If its not powered on yet - then error
	if(! dc->poweredOn)
		return false;

	boolean rtn =  droneCellClaimAttempt(dc,DC_SMS_INIT_TRY);
	if(rtn){
		// Save the number to call
		strncpy(dc->callNumber,callNumber,MAX_PHONE_LENGTH);
		dc->callNumber[MAX_PHONE_LENGTH]='\0';

		// Flush the SMS buffer
		bufferFlush(dc->smsbuf);

		// We are in SMS writing mode
		dc->smsMode = TRUE;

	}
	return rtn;

}

// --------------------------------------------------------------------

// Send the message from the buffer

// Response should be:
// +CMGS: <msg number>

// Return message number or -1 if there is a comms error


static int _droneCellSMSSendProcess(DRONECELL* dc){
	Reader	reader;
	Writer  writer;
	int 	ch;
	uint8_t response;
	int		msgNumber = -1;

nextstep:
	response = DC_RESPONSE_OK;

	switch(dc->step++){
		// Prepare dronecell to write to SMS memory
		case 0:
			// If there is no text to send then just exit
			if(bufferBytesUsed(dc->smsbuf)==0){
				return -1;
			}


			// Send the command to the drone cell
			OUTPUT(droneCellGetWriter(dc)){ 				// Send rprintf to the DroneCell
				rprintf("AT+CMGW=\"%s\"\r",dc->callNumber);		// write to memory
			}
			response = droneCellGetResponse(dc,2);	// Should be '> '
			response = (response == '>') ? DC_RESPONSE_OK : DC_RESPONSE_ERROR;

			if(response != DC_RESPONSE_OK){
				if(dc->retries++ < 5){
					// Ok lets retry this step
					dc->step--;
					goto nextstep;
				}

				// We have retried enough
//				rprintf("Failed AT+CMGW\n");
				return -1;
			}

			// Response is ok so far
			goto nextstep;

		case 1:
			// Write the text from the buffer to the device

			// We cannot do any retries after this point since the
			// text has been removed from the buffer

			// Get a reader to read the user text from the buffer and a
			// writer to write to the drone cell
			reader = droneCellGetSMSReader(dc);
			writer = droneCellGetWriter(dc);
			while(ch = reader(), ch != -1){
				writer( (uint8_t) ch);
			}

			_droneCellStartListening(dc);				// start listening again

			writer(0x1A);								// Append a ctrl Z

			// We are starting to get a response - so get it all
			response = droneCellGetResponse(dc, 20);

			if(response != DC_RESPONSE_TIMEOUT){
				if(strncmp_P(dc->buffer,PSTR("+CMGW: "),7)==0){
					// Get the response msg
					msgNumber = atoi(&dc->buffer[7]);
#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"Save Msg No:%d\r\n",msgNumber);
#endif
				}else{
#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"Save Returned '%s'\r\n",dc->buffer);
#endif
				}
			}else{
#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"Save Response '%s'\r\n",dc->buffer);
#endif
			}

	} // End switch
	return msgNumber;
}

// Send the current message text
// ESC quits without sending
boolean droneCellSMSSend(DRONECELL* dc){
	boolean rtn = FALSE;
	if(dc->smsMode && dc->process == DC_SMS_INIT_TRY){

		// Change mode to sending the SMS to the device
		dc->process = DC_SMS_SEND_TRY;

		// Reset the process variables
		dc->step = 0;
		dc->retries=0;

		// start scheduling the process sequence
		int msgNo = _droneCellSMSSendProcess(dc);
		if(msgNo!=-1){
			rtn = TRUE;
		}

		// Cannot call me again
		dc->smsMode = FALSE;

		droneCellRelease(dc, (rtn) ? DC_ERROR_NONE : DC_ERROR_SMS);
	}
	return rtn;
}
