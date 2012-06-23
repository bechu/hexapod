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
 *        File: DroneCellQueue.c
 *  Created on: 9 Apr 2011
 *      Author: Clive Webster
 *
 *
 *  This process is constantly running in the background
 *  It will do things like sending any SMS file, getting signal strength etc
 *
 */
#include "../DroneCell.h"
#include <stdlib.h>
#include <string.h>
#include "../../core.h"
#include "../../timer.h"


typedef enum {
	TIMEOUT,
	EMPTY_MESSAGE,
	INVALID_INDEX,
	OUTGOING_SENT,
	OUTGOING_UNSENT,
	INCOMING_UNREAD
} MSG_TYPE;

/**
 * Do each background step
 */

static uint8_t startMsgIndex;


#define GET_SEND_RESPONSE 100

/**
 * Read a message. The device must already be claimed.
 * This will return immediately
 */
static MSG_TYPE droneCellGetMessage(DRONECELL* dc, uint8_t msgIndex){
	MSG_TYPE msgStatus = TIMEOUT;
	uint8_t response = DC_RESPONSE_OK;

	int step = 0;
	int inx = 0;
	Reader reader= droneCellGetReader(dc);
	Writer writer = droneCellGetSMSWriter(dc);
	bufferFlush(dc->smsbuf);

	TICK_COUNT start = clockGetus();		// Get start time
	TICK_COUNT timeout = 1000000U;			// timeout = 1 second

	__uartFlushReceiveBuffer(dc->uart);
	_droneCellStartListening(dc);			// start listening again

	OUTPUT(droneCellGetWriter(dc)){ 			// Send rprintf to the DroneCell
		rprintf("AT+CMGR=%u,1\r",(unsigned int)msgIndex);		// Read message without changing status
	}

	// Process the input
	do{
		int ch = reader();
		if(ch!=-1 && ch!='\n'){
			// Process next character
			char c = (char)ch;
			switch(step){

			case 0:
				// Should be '+CMGR:' or '4' if error, or '0' if its blank
				if(c!='+'){
					while(reader()!='\r');			// skip to end of line
					msgStatus = (c=='0') ? EMPTY_MESSAGE : INVALID_INDEX;
					goto exit;						// restart the loop
				}else{
					step++;
				}
				break;

			case 1:
				// Skip to open quote of status
				if(c == '"'){
					inx = 0;
					dc->buffer[inx] = '\0';
					step++;
				}
				break;

			case 2:
				// Store to close quote of status
				if(c != '"'){
					dc->buffer[inx++] = c;
					dc->buffer[inx] = '\0';
				}else{
					// got end of status
					step++;
					if( strcmp_P(dc->buffer, PSTR("STO UNSENT")) == 0 ){
						//Its an unsent message
						msgStatus = OUTGOING_UNSENT;
					}else if( strcmp_P(dc->buffer, PSTR("STO SENT")) == 0 ){
						//Its an sent message
						msgStatus = OUTGOING_SENT;
					}else if( strcmp_P(dc->buffer, PSTR("REC UNREAD")) == 0 ){
						//Its an sent message
						msgStatus = INCOMING_UNREAD;
					}else{
#ifdef DEBUG_DRONECELL
						PRINTF(DEBUG_DRONECELL,"\n**Unexpected Status in slot %d='%s'**\n",msgIndex,dc->buffer);
#endif
					}
				}
				break;

			case 3:
				// Skip to open quote of phone number
				if(c == '"'){
					inx = 0;
					dc->buffer[inx] = '\0';
					step++;
				}
				break;

			case 4:
				// Store to close quote of phone number
				if(c != '"'){
					dc->buffer[inx++] = c;
					dc->buffer[inx] = '\0';
				}else{
					// got end of phone number
					step++;
					strcpy(dc->callNumber,dc->buffer);
				}
				break;

			case 5:
				// Ignore rest of line
				if(c == '\r') step++;
				break;

			case 6:
				// Store char to message
				if(c != '\r'){
					writer(c);
				}else{
					// End of message
					step++;
					response = droneCellGetResponse(dc,20);
					goto exit;
				}
				break;

			default:
				goto exit;

			} // end switch
		} // endif got a char

		// See if we have timed out
		if(clockHasElapsed(start,timeout)){
			response = DC_RESPONSE_TIMEOUT;
		}

	}while(response==DC_RESPONSE_OK);

exit:
	_droneCellStopListening(dc);			// stop listening


	return msgStatus;
}

// Device must already be claimed
static uint8_t deleteMsg(DRONECELL* dc, uint8_t msgIndex){
	OUTPUT(droneCellGetWriter(dc)){ 					// Send rprintf to the DroneCell
		rprintf("AT+CMGD=%u\r",(unsigned int)msgIndex);	// Delete the message from SIM
	}
	return droneCellGetResponse(dc,255);				// get response and stop listening
}

static void _droneCellQueue(SchedulerData data, TICK_COUNT lastTime, TICK_COUNT overflow){
	DRONECELL* dc = data;						// Which DroneCell are we working with ?
	TICK_COUNT delay;
	uint8_t response;
	cBuffer* rxBuf;


nextstep:
	delay = 500000;					// Default delay is 0.5s
	response = DC_RESPONSE_OK;


	if(! dc->poweredOn){
		dc->qstep = 0;
	}else{
		switch(dc->qstep){

		case 0:
			// try to get the signal strength
			if(droneCellClaimAttempt(dc,DC_SIGNAL_TRY)){
				response = droneCellSendATcommand(dc,PSTR("+CSQ"));
				if(response != DC_RESPONSE_TIMEOUT){
					char* ptr = (char*)(dc->buffer);
					if( strncmp_P(dc->buffer, PSTR("+CSQ:"),5 ) == 0){
						// We have got a response
						ptr+=5;
						while(*ptr==' ') ptr++;
						dc->signalStrength = atoi(ptr);
					}
				}
				droneCellRelease(dc,(response==DC_RESPONSE_OK) ? DC_ERROR_NONE : DC_ERROR_SIGNAL);
				dc->qstep++;
			}
			break;

		case 1:
			// Get the network connection status
			// Response is: "+CREG: n,y"
			// y = '0' = Not registered, not searching for new operator
			// y = '1' = Connected to home network
			// y = '2' = Not registered, is searching for new operator
			// y = '3' = Registration denied
			// y = '4' = Unknown
			// y = '5' = Registered, roaming
			if(droneCellClaimAttempt(dc,DC_REGISTERED_TRY)){
				response = droneCellSendATcommand(dc,PSTR("+CREG?"));
				if(response == DC_RESPONSE_OK){
					int inx;
					if(strncmp_P(dc->buffer,PSTR("+CREG: "),7)==0){
						inx = 7;
						while(dc->buffer[inx++]!=',');
						dc->registered = dc->buffer[inx];

					}else{
						response = DC_RESPONSE_ERROR;
					}
				}
				droneCellRelease(dc,(response==DC_RESPONSE_OK) ? DC_ERROR_NONE : DC_ERROR_REGISTERED);
				dc->qstep++;

				// If we've stopped looking for a signal for 60 seconds or more then reboot the drone cell
				if(dc->registered == DC_REGISTERED_NO || dc->registered == DC_REGISTERED_DENIED){
					if(clockHasElapsed(dc->nonRegStart, 60000000)){
						// It has stopped searching for a network for 60 seconds or more
						// Try to turn it on again
						dc->poweredOn = FALSE;
						dc->nonRegStart = lastTime;
						droneCellPowerOn(dc);
					}
				}else{
					dc->nonRegStart = lastTime;
				}
			}
			break;

		case 2:
			// Try to get the name of the network operator
			if( droneCellClaimAttempt(dc, DC_OPERATOR_TRY) ){
				// the device is mine
				// Put name of operator, response is '+COPS: <num>,<num>,"operator" ' or to start may be '+COPS: <num>'
				// The operator name can be up to 16 characters - defined as MAX_OPERATOR
				char *name = null;
				if(droneCellSendATcommand(dc,PSTR("+COPS?"))==DC_RESPONSE_OK && strncmp_P(dc->buffer,PSTR("+COPS: "),7)==0){
					int len = strlen(dc->buffer);
					for(int inx = 7; inx < len; inx++){
						if(dc->buffer[inx]=='"'){
							// Copy the name up to '"' or null
							name = &dc->buffer[++inx];
							while(dc->buffer[inx] && dc->buffer[inx]!='"') inx++;
							dc->buffer[inx]='\0';
							CRITICAL_SECTION{
								strncpy(dc->operatorName, name,DC_MAX_OPERATOR);
								dc->operatorName[DC_MAX_OPERATOR] = '\0';
							}
							break;
						}
					}
				}
				if(name==null){
					dc->operatorName[0]=0;
				}
				droneCellRelease(dc, DC_ERROR_NONE);
				dc->qstep++;
			}
			break;

		case 3:
			startMsgIndex = dc->qstep++;
			goto nextstep;

		default:
			if(!droneCellIsRegistered(dc)){
				dc->qstep = 0;
				goto reschedule;
			}

			// Process a message

			if( droneCellClaimAttempt(dc, DC_READ_TRY) ){
				// Get the message index to read
				dc->msgIndex = dc->qstep++ - startMsgIndex;

				MSG_TYPE msgStatus = droneCellGetMessage(dc,dc->msgIndex);
				if(msgStatus==INVALID_INDEX){
					// Start from beginning again
					delay = 10000000U;			// wait for 10 seconds
					dc->qstep = 0;
				}else if(msgStatus == TIMEOUT || msgStatus==EMPTY_MESSAGE){
					// try the next message
				}else if(msgStatus == INCOMING_UNREAD){
					// Mark this as the next unread msg then move to the next
					if(dc->indexReceived==0){
						dc->indexReceived = dc->msgIndex;
					}
				}else if(msgStatus == OUTGOING_UNSENT){
					if(droneCellIsRegistered(dc)){
						// Try to send
						OUTPUT(droneCellGetWriter(dc)){ 					// Send rprintf to the DroneCell
							rprintf("AT+CMSS=%u,\"%s\"",
									(unsigned int)dc->msgIndex,				// Send from memory slot
									dc->callNumber);						// to the stored tel number
							_droneCellStartListening(dc);					// Start listening to response
							rprintfChar('\r');									// Start the send
						}
						// Response should be '+CMGS: <msgno>'
						// Exit without releasing
						dc->qstep = GET_SEND_RESPONSE;
						dc->retries = 0;
						goto nextstep;

					}
				}else if(msgStatus == OUTGOING_SENT){
					// Just delete the sent message
					deleteMsg(dc,dc->msgIndex);
				}

				// Release it
				droneCellRelease(dc,DC_ERROR_NONE);
			}
			break;

		case GET_SEND_RESPONSE:
			// We are waiting for a reply about sending an SMS message
			// The device is still claimed and we are listening
			rxBuf = dc->uart->rxBuffer;
			if(bufferBytesUsed(rxBuf)==0 && ++dc->retries < 100){
				// No response yet
				delay = 500000;								// retry in 500ms, hence timeout is 50 seconds
			}else{
				response = droneCellGetResponse(dc,255);	// get response and stop listening

				if(response == DC_RESPONSE_OK){
#ifdef DEBUG_DRONECELL
					PRINTF(DEBUG_DRONECELL,"** Sent msg %u**\r\n",(unsigned int)dc->msgIndex);
#endif
					// Now delete the msg
					response = deleteMsg(dc,dc->msgIndex);	// This will stop the listening
#ifdef DEBUG_DRONECELL
					PRINTF(DEBUG_DRONECELL,"**Delete Msg:%u - %s **\r\n",(unsigned int)dc->msgIndex,(response==DC_RESPONSE_OK) ? "OK" : "FAILED");
#endif
				}else{
#ifdef DEBUG_DRONECELL
					PRINTF(DEBUG_DRONECELL,"** Send Failed %u***\r\n",(unsigned int)response);
#endif
				}

				// Start process over again
				dc->qstep = 0;

				// Release the device
				droneCellRelease(dc,DC_ERROR_NONE);
			}
			break;

		} // end switch


		if(response!=DC_RESPONSE_OK){

			delay=1000000;				// Wait for 1 second
#ifdef DEBUG_DRONECELL
			PRINTF(DEBUG_DRONECELL,"** Retry Queue Step: %d **\r\n",(int)(dc->qstep));
#endif

		}
	}

reschedule:
	// We always need to schedule the next step
	scheduleJob(&_droneCellQueue,dc,			// Run me again after the delay
							lastTime,delay);

}

void droneCellQueue(DRONECELL *dc){
	dc->qstep = 0;
	scheduleJob(&_droneCellQueue,dc,clockGetus(),10000000U);	// Start queue 10 seconds after power up
}

// The user has requested to read a given message index
boolean droneCellReadMessage(DRONECELL* dc, uint8_t msgIndex){
	boolean rtn = FALSE;

	if(droneCellClaimAttempt(dc,DC_READ_TRY)){
		dc->incomingPhoneNumber[0] = '\0';
		dc->incomingMessage[0] = '\0';

		// Read the message
		MSG_TYPE typ = droneCellGetMessage(dc,msgIndex);
		if(typ == INCOMING_UNREAD){
			// Copy the phone number
			strcpy(dc->incomingPhoneNumber, dc->callNumber);
			// Copy the message
			Reader rdr = droneCellGetSMSReader(dc);

			int ch;
			int inx=0;
			while(ch=rdr(),ch!=-1){
				dc->incomingMessage[inx++] = (char)ch;
			}
			dc->incomingMessage[inx] = '\0';
			rtn = TRUE;

		}

		// Release the device
		droneCellRelease(dc,DC_ERROR_NONE);

	}
	return rtn;
}

boolean droneCellDeleteMessage(DRONECELL* dc, uint8_t msgIndex){
	boolean rtn = FALSE;

	if(droneCellClaimAttempt(dc,DC_DELETE_TRY)){
		uint8_t response = deleteMsg(dc,msgIndex);
		if(response == DC_RESPONSE_OK){
			rtn = TRUE;
			if(dc->indexReceived==msgIndex){
				dc->indexReceived = 0;
			}
		}

		// Release the device
		droneCellRelease(dc,(response==DC_RESPONSE_OK) ? DC_ERROR_NONE : DC_ERROR_DELETE);
	}
	return rtn;
}
