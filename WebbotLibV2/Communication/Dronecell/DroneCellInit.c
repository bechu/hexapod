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
 *        File: DroneCellInit.c
 *  Created on: 1 Apr 2011
 *      Author: Clive Webster
 */

#include "../DroneCell.h"
#include "../../core.h"
#include "../../buffer.h"
#include <stdlib.h>

#define BUFSIZE 48

// Receive handler to just throw chars away
static void dispose(unsigned char c, void *dc){
#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"[%c]",c);
#endif
}


/*
 * Send a byte to the dronecell and any logger
 */
static uint8_t write(DRONECELL* dc, uint8_t byte){
	return (droneCellGetWriter(dc))(byte);
}


/*
 * Reads a byte from the dronecell but ignoring any logger
 */
int droneCellGetByte(DRONECELL* dc){
	Reader r = __uartGetReader(dc->uart);
	return r();
}

/*
 * Send a byte to the dronecell but ignoring any logger
 */
uint8_t droneCellSendByte(DRONECELL* dc, uint8_t byte){
	Writer w = __uartGetWriter(dc->uart);
	w(byte);
	return byte;
}

boolean droneCellIsBusy(const DRONECELL* dc){
	return (dc->process == DC_OK ) ? FALSE : TRUE;
}


/**
 * Current task has finised - so return a value
 */
void droneCellRelease(DRONECELL*dc , DroneCellResult result){
	_droneCellStopListening(dc);
	__uartFlushReceiveBuffer(dc->uart);
	dc->result = result;		// Save result of the last call
	dc->process = DC_OK;		// Nothing running
}

/*
 *
 *  Try to claim the DroneCell.
 *  If another task has already claimed it then return false
 *  else return true and the DroneCell now belongs to the caller
 */
boolean droneCellClaimAttempt(DRONECELL* dc, DroneCellState newState) {
	boolean mine = FALSE;

	CRITICAL_SECTION{
		if(!droneCellIsBusy(dc)){
			// Claim it
			dc->process = newState;

			// clear retries
			dc->retries = 0;

			// Starting at first step of the process
			dc->step = 0;

			// Exit the loop
			mine = TRUE;
		}
	}
	return mine;
}

/*
 *  Wait for any previous command to finish and then allocate
 *  the device to me using the new state
 *
 *  Return: TRUE if all is ok, FALSE if there is an error
 */
void droneCellClaim(DRONECELL* dc, DroneCellState newState){
	do{
		breathe();				// allow background tasks to work
	} while(droneCellClaimAttempt(dc,newState)== FALSE);
	// The device now belongs to the caller
}

void _droneCellStopListening(DRONECELL *dc){
	__uartAttach(dc->uart, &dispose, dc); 			// stop listening
}

/*
 * The response can be 1 or 2 lines each terminated by a CR
 * For a single line message it is just 'short code byte', CR
 * For a two line message it is: multi-character line 1, CR
 * 								 'short code byte', CR
 *
 * For a 2 line message we will return the first line of text in
 *
 * Return the short message response code
 * one of the DC_RESPONCE_xxxx constants
 */
uint8_t droneCellGetResponse(DRONECELL* dc, uint8_t maxLen){
//	UART* uart = dc->uart;
	TICK_COUNT timeout = 1000000;			// default is 1 second

	if(maxLen >= BUFSIZE){
		maxLen = BUFSIZE - 1;
	}

	_droneCellStartListening(dc);			// start listening again


	Reader reader= droneCellGetReader(dc);

	// Now read the response either <cr><lf>text<cr><lf> or <cr>code<cr>
	uint8_t inx =  0;
	uint8_t inx2 = 0;						// There is no line 2
	char* buffer = dc->buffer;

	buffer[0] = 0;							// Blank out the responses

	TICK_COUNT start = clockGetus();		// Get start time

//	if(dc->process == DC_SMS_SEND_TRY){
//		timeout = 45000000;					// 45sec timeout when sending msg
//	}

	do{
		int ch = reader();
		if(ch!=-1){
			if(ch == '\n'){
				// ignore it
			}else if(ch == '\r'){
				if(inx == 0){
					continue;	// Starts with a '\r' ? Shouldn't happen ie blank line, so ignore it
				}
				if(inx2 == 0){
					// We are processing line1
					char ch0 = buffer[0];
					if(inx==1 && ch0>='0' && ch0<='9'){
						// It is a single line message that has now finished
						goto exit;
					}

					// Its the end of line 1 for a multi line message
					inx2 = ++ inx;			// Mark start of line 2
					buffer[inx] = '\0';		// Terminate the second string

				}else{
					// We are processing line2 and it has now finished
					goto exit;
				}
			}else if(ch < 128){
				// Append the 7bit character
				buffer[inx++] = (char)ch;
				buffer[inx] = 0;

				// Exit if we have run out of buffer space
				if(inx == maxLen){
					goto exit;
				}
			}
		}
	}while(!clockHasElapsed(start,timeout));

//timeout:
#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"<=timeout\r\n");
#endif
	buffer[inx2]=0;			// return a blank message
exit:

	_droneCellStopListening(dc);	// stop listening

#ifdef DEBUG_DRONECELL
	if(buffer[inx2]){
		if(inx2 != 0){
			PRINTF(DEBUG_DRONECELL,"<=Response: %c -'%s'\r\n",buffer[inx2],buffer);
		}else{
			PRINTF(DEBUG_DRONECELL,"<=Response: %c\r\n",buffer[inx2]);
		}
	}
#endif


	// Return the response code
	return buffer[inx2];

}

/*
 * Return 0 if timeout, else returns first char from response is in buffer
 */
uint8_t droneCellSendATcommand(DRONECELL* dc, const char* str){

	UART* uart = dc->uart;

#ifdef DEBUG_DRONECELL
	PRINTF(DEBUG_DRONECELL,"=>AT");
	if(str){
		fprintf_P(DEBUG_DRONECELL,str);
	}
	PRINTF(DEBUG_DRONECELL,"\r\n");
#endif

	// Flush receive buffer
	__uartFlushReceiveBuffer(uart);

	write(dc, 'A');
	write(dc, 'T');

	if(str){
		uint8_t ch;
		while(ch = pgm_read_byte(str++), ch!=0){
			write(dc, ch);
		}
	}

	_droneCellStartListening(dc);			// start listening again

	// End the command
	write(dc, '\r');

	return droneCellGetResponse(dc, BUFSIZE-1);
}




void _droneCellStartProcess(DRONECELL* dc, SchedulerCallback process, DroneCellState state){

	// Wait for the previous process to finish
	droneCellClaim(dc, state);

	// start scheduling the process sequence
	scheduleJob(process,dc,clockGetus(),0);

}

void droneCellInit(DRONECELL* dc){

	if(dc->buffer==null){
		dc->buffer     = malloc(BUFSIZE);
		dc->callNumber = malloc(MAX_PHONE_LENGTH+1);			// Maximum length of a phone number
		dc->incomingPhoneNumber = malloc(MAX_PHONE_LENGTH+1);	// Maximum length of a phone number
		dc->incomingMessage = malloc(1 + dc->smsbuf->size);		// Use the same size as given in PD
		dc->operatorName = malloc(DC_MAX_OPERATOR+1);				// Maximum length of a service operator name
		dc->operatorName[0] = 0;
	}

	// Initialise the UART
	_droneCellStopListening(dc); 						// throw away any received chars
	_uartInit(dc->uart,dc->baud);

	// Rest are input sensors
	pin_make_input(dc->ringPin, FALSE);
	pin_make_input(dc->simPin, FALSE);
	pin_make_input(dc->presentPin, FALSE);

	// Starts with the power off
	dc->poweredOn = false;
	dc->process = DC_OK;

	// Try to turn it on
	droneCellPowerOn(dc);

	// Start the queue that receives or sends SMS messages
	droneCellQueue(dc);

}
