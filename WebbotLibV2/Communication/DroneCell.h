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
 *        File: DroneCell.h
 *  Created on: 1 Apr 2011
 *      Author: Clive Webster
 */

#ifndef DRONECELL_H_
#define DRONECELL_H_

#include "../libdefs.h"
#include "../_uart_common.h"
#include "../iopin.h"
#include "../clock.h"
#include "../scheduler.h"
#include "../rprintf.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif


// The maximum phone number length ignoring anything that is not a + or 0...9
#define MAX_PHONE_LENGTH 16

// The maximum length of the operator name
#define DC_MAX_OPERATOR 16

// The state machine
typedef enum {
	// 0 = all ok
	DC_OK = 0,						// Ready for next cmd

	// +ve values = stuff is happening
	DC_POWER_UP_TRY = 1,				// DroneCell is trying to power up
	DC_POWER_DOWN_TRY,					// DroneCell is powering down
	DC_SMS_INIT_TRY,					// DroneCell is trying to init SMS mode
	DC_SMS_SEND_TRY,					// Trying to save the SMS message
	DC_SIGNAL_TRY,						// Trying to get the signal strength
	DC_REGISTERED_TRY,					// Trying to determine if we are registered with an operator
	DC_OPERATOR_TRY,					// Trying to get the current operator
	DC_DELETE_TRY,						// Trying to delete a queued message
	DC_READ_TRY							// Trying to read any queued commands
} DroneCellState;

// The result of the last operation passed back using
//    droneCellRelease()
typedef enum {
	DC_ERROR_NONE = 0,
	DC_ERROR_STARTUP,				// Error on startup of the device, device not present
	DC_ERROR_SIM,					// Error reading SIM card
	DC_ERROR_SIM_MISSING,			// No SIM card is inserted
	DC_ERROR_SMS,					// We cannot enter SMS mode
	DC_ERROR_SIGNAL,				// We cannot read the signal
	DC_ERROR_REGISTERED,			// We cannot read the registered state
	DC_ERROR_READ,					// We cannot read the messages from the SIM card
	DC_ERROR_DELETE					// We cannot delete the messages from the SIM card
} DroneCellResult;

// The result of the message that has just completed


/* 	---------------------------------------------------
 *
 *  Define the structure for a DroneCell
 *
	--------------------------------------------------- */
typedef struct s_dronecell{
	UART* 			uart;							// The uart to use
	BAUD_RATE 		baud;							// The baud rate to use
	PGM_P			smsc;							// SMS Message center in progmem
	const IOPin*	resetPin;						// The power reset output pin
	const IOPin*	ringPin;						// The ring detect input pin
	const IOPin*	simPin;							// The SIM card detect input pin
	const IOPin*	presentPin;						// The DroneCell Present status pin

	cBuffer*		smsbuf;							// A buffer for reading/writing SMS messages

	Writer			writer;							// The writer to send out a byte to the DroneCell and any logger
	Reader			reader;							// The reader to read responses from the DroneCell and write to any logger

	volatile DroneCellState	process;					// What operation am I current doing?
	volatile DroneCellResult result;				// Result of the last operation

	volatile uint8_t qstep;							// Current step for the background queue
	volatile uint8_t step;							// Any step for this state
	volatile uint8_t retries;						// The retry counter
	volatile uint8_t msgIndex;						// The msg index being processed in the bkgnd

	char *	     	 buffer;						// Response buffer

	// Various flags - dont change in your application code but you can read them
	volatile boolean poweredOn;						// Is it powered on and ready to go?
	volatile boolean smsMode;						// Are we in SMS text mode?
	volatile uint8_t signalStrength;				// The current signal strength. 0=none
	volatile char	 registered;					// The DC_REGISTERED_xxxx

	char*			 callNumber;					// The number to call
	char*	 		 operatorName;					// The name of the current operator
	volatile uint8_t indexReceived;					// The index of the first unused msg 0=none

	// Private copy for any droneCellReadMessage commands
	char*			incomingPhoneNumber;				// The phone number it was from
	char*			incomingMessage;				// The message body
	TICK_COUNT		nonRegStart;
	boolean			forceReboot;
} DRONECELL;

// The status values returned when talking to the device
#define DC_RESPONSE_TIMEOUT 0
#define DC_RESPONSE_OK '0'
#define DC_RESPONSE_CONNECT '1'
#define DC_RESPONSE_RING '2'
#define DC_RESPONSE_NO_CARRIER '3'
#define DC_RESPONSE_ERROR '4'
#define DC_RESPONSE_NO_DIALTONE '6'
#define DC_RESPONSE_BUSY '7'
#define DC_RESPONSE_NO_ANSWER '8'
#define DC_RESPONSE_PROCEEDING '9'

// Is it registered with the phone network (response to AT+CREG call);
#define DC_REGISTERED_NO  		'0'	/* Not registered, not searching for new operator */
#define DC_REGISTERED_YES 		'1' /* Registered with home network */
#define DC_REGISTERED_SEARCHING '2' /* Not registered, is searching for new operator */
#define DC_REGISTERED_DENIED 	'3' /* Registration denied */
#define DC_REGISTERED_UNKNOWN 	'4' /* unknown */
#define DC_REGISTERED_ROAMING 	'5' /* Registered, roaming */


// DRONECELL DroneCell = MAKE_DRONECELL(DroneCell_uart,38400,E2,null,null,null);

#define MAKE_DRONECELL(uart,baud,smsc,resetPin,ringPin,simPin,statusPin,smsbuf,writer,reader){ &((uart)->_uart_),baud,smsc,resetPin,ringPin,simPin,statusPin, smsbuf, writer, reader, \
		DC_OK, 				/* Initial state */  \
		DC_ERROR_NONE,		/* result of last process */ \
		0,					/* Queue Step */ \
		0,					/* Step */ \
		0, 					/* Retries */ \
		0, 					/* MsgIndex */ \
		null, 				/* buffer allocated at runtime */  \
		false, 				/* Not yet powered on */ \
		false,				/* Not yet in SMS mode */ \
		0,					/* The current signal strength */ \
		DC_REGISTERED_UNKNOWN, /* Not yet registered with the network*/ \
		null, 				/* The telephone number to call */ \
		null,				/* The operator name */ \
		0,					/* No message has been received */ \
		null,null,			/* Both incoming fields are blank */ \
		0,FALSE				/* nonRegStart */ \
}


// Initialise the DroneCell on power up
extern void droneCellInit(DRONECELL* dc);

// Power up the DroneCell - called by droneCellInit but you
// may want to call it after droneCellPowerOff
extern void droneCellPowerOn(DRONECELL* dc);

// Power off the DroneCell to save power
// This will wait until any current processing is finished before powering down
// Wake it up again using droneCellPowerOn
extern void droneCellPowerOff(DRONECELL* dc);

// Initialise for sending a text message
// Return true if ok, false if failed
// If all is ok then use:-
// 		DRONECELL_WRITE_SMS(&myDroneCell){
// 			rprintf("Hello World");
//			... any other rprintf stuff ...
//			}
// 		droneCellSMSSend(&myDroneCell); // Send the text

extern boolean droneCellSMSInit(DRONECELL* dc, const char callNumber[])  __attribute__ ((warn_unused_result));

// Try to send the message, must have called droneCellSMSInit before hand
extern boolean droneCellSMSSend(DRONECELL* dc);

/*
 * generic routine to send a char to a Drone Cell bypassing any logger
 * If you want it to be logged then use droneCellGetWriter(dc)(char)
 */
extern uint8_t droneCellSendByte(DRONECELL* dc, uint8_t byte);

/*
 * generic routine to read a char from a Drone Cell bypassing any logger
 * If you want it to be logged then use droneCellGetReader(dc)()
 * Returns -1 if no character, else low 8 byte has the value
 */
extern int droneCellGetByte(DRONECELL* dc);

/*
 * Read an incoming message from the SIM card
 * Return: TRUE if this is an incoming msg, FALSE if its outbound or empty or the device is busy
 * 		   If TRUE then incomingPhoneNumber and incomingMessage have the content
 *
 * You can reply to the message using droneCellSMSInit and droneCellSMSSend
 *
 */
extern boolean droneCellReadMessage(DRONECELL* dc, uint8_t msgIndex);

extern boolean droneCellDeleteMessage(DRONECELL* dc, uint8_t msgIndex);

// ----------------- Read some memory variables ie nice and quick ---------------

// Get new value for the signal strength
// 0 = none
// 1= -111dBm
// 2..30 = -109dBm ... -53dBm
// 31 = -51dBm or better
// 99 = not known
// The returned value is in the 'signalStrength' data member
static __inline__ uint8_t droneCellGetSignalStrength(const DRONECELL* dc){
	return dc->signalStrength;
}

// Is the drone cell registered with the network operator?
static __inline__ boolean droneCellIsRegistered(const DRONECELL* dc){
	return (dc->registered==DC_REGISTERED_YES || dc->registered==DC_REGISTERED_ROAMING) ? TRUE : FALSE;
}

// Return 0 if no message waiting, else index of message to use in droneCellReadMessage
static __inline__ uint8_t droneCellMessageWaiting(const DRONECELL* dc){
	return dc->indexReceived;
}

// Get the name of the service operator or '' if not yet registered
static __inline__ const char* droneCellGetOperator(const DRONECELL* dc){
	return dc->operatorName;
}


// Return a Writer that can be used to write to the SMS buffer
static __inline__ Writer droneCellGetSMSWriter(const DRONECELL* dc){
	return bufferGetWriter(dc->smsbuf);
}

// Return a Reader that can be used to read from the SMS buffer
static __inline__ Reader droneCellGetSMSReader(const DRONECELL* dc){
	return bufferGetReader(dc->smsbuf);
}

// ----- Helper macros -------

// Send all rprintf text to the SMS body and restore afterwards
#define DRONECELL_WRITE_SMS(device) OUTPUT(droneCellGetSMSWriter(device))


// ----- Routines for use only by the library -------
#ifdef BUILDING_LIBRARY
//#define DEBUG_DRONECELL  stdout

extern void _droneCellStartProcess(DRONECELL* dc, SchedulerCallback process, DroneCellState state);
extern uint8_t droneCellSendATcommand(DRONECELL* dc, const char* str);

// Process the response from the device
extern uint8_t droneCellGetResponse(DRONECELL* dc, uint8_t maxLen);

// Return a Writer that can be used to write to the device and any loggers
static __inline__ Writer droneCellGetWriter(const DRONECELL* dc){
	return dc->writer;
}


// Return a Reader that can be used to read chars from the device and echo them to any loggers
static __inline__ Reader droneCellGetReader(const DRONECELL* dc){
//	return __uartGetReader(dc->uart);
	return dc->reader;
}

// Return TRUE if we are currently sending/processing a command in the background,
// ie don't start a new one until the last one is finished
//
extern boolean droneCellIsBusy(const DRONECELL* dc);

// Wait until the DroneCell has finished processing
// the current command and then claim it
// Do NOT call this from your own background routine (interrupt) routine as it will hang forever
// Instead you should use droneCellIsBusy and only start a message if that returns FALSE
extern void droneCellClaim(DRONECELL* dc, DroneCellState newState);

extern boolean droneCellClaimAttempt(DRONECELL* dc, DroneCellState newState)  __attribute__ ((warn_unused_result));

// We have just finished a process and are passing back the reult
extern void droneCellRelease(DRONECELL*dc , DroneCellResult result);

static __inline__ void _droneCellStartListening(DRONECELL *dc){
	__uartDetach(dc->uart); 					// start listening again
}

extern void _droneCellStopListening(DRONECELL *dc);

// Start running the queue
extern void droneCellQueue(DRONECELL *dc);

#endif
// ----- Routines for use only by the library -------
#ifdef __cplusplus
}
#include "../Stream/Stream.h"
// ---------------------- C++ code --------------------
class DroneCell : public Stream{
public:
	DroneCell(DRONECELL* cstr){
		m_cstr = cstr;
	}

	operator DRONECELL*() const{
		return m_cstr;
	}

	// Start the power on cycle
	void powerOn(void) const{
		droneCellPowerOn(m_cstr);
	}

	// Start the power off cycle
	void powerOff(void) const{
		droneCellPowerOff(m_cstr);
	}

	// Get the current signal strength
	uint8_t getSignalStrength(void) const{
		return droneCellGetSignalStrength(m_cstr);
	}

	// Are we currently registered with a network operator
	boolean isRegistered(void) const{
		return droneCellIsRegistered(m_cstr);
	}

	// Return 0 if no message waiting, else index of message to use in readMessage
	uint8_t messageWaiting(void) const{
		return droneCellMessageWaiting(m_cstr);
	}

	// Get the name of the service operator or '' if not yet registered
	const char* getOperator(void) const{
		return droneCellGetOperator(m_cstr);
	}

	// Try to read the given msg
	boolean readMessage( uint8_t msgIndex) const{
		return droneCellReadMessage(m_cstr,msgIndex);
	}

	// Try to delete the given msg
	boolean deleteMessage( uint8_t msgIndex) const{
		return droneCellDeleteMessage(m_cstr,msgIndex);
	}

	char* getMessagePhoneNumber(void) const {
		return m_cstr->incomingPhoneNumber;
	}

	char* getMessageBody(void) const {
		return m_cstr->incomingMessage;
	}

	/* Implement the raw methods from Stream */
	// Stream writes to the SMS message
	virtual int write(uint8_t data){
		if(!m_cstr->smsMode){
			return EOF;
		}
		return droneCellGetSMSWriter(m_cstr)(data);
	}
	virtual int read(void){
		return EOF;
	}

	boolean initSMS(const char callNumber[]) const   __attribute__ ((warn_unused_result)){
		return droneCellSMSInit(m_cstr, callNumber);
	}

	// Try to send the message, must have called initSMS before hand
	boolean sendSMS(void) const{
		return droneCellSMSSend(m_cstr);
	}

	boolean isPoweredUp(void) const{
		return m_cstr->poweredOn;
	}

private:
	DRONECELL* m_cstr;
};
#endif


#endif /* DRONECELL_H_ */
