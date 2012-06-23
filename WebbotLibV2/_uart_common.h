/*
 * $Id: _uart_common.h,v 1.13 2010/06/14 19:21:25 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _uart_common.h,v $
 * Revision 1.13  2010/06/14 19:21:25  clivewebster
 * Add copyright license info
 *
 * Revision 1.12  2010/04/25 03:33:56  clivewebster
 * Handle receive buffer calls better when there is no buffer
 *
 * Revision 1.11  2010/03/24 19:48:54  clivewebster
 * *** empty log message ***
 *
 * Revision 1.10  2010/03/07 19:32:43  clivewebster
 * *** empty log message ***
 *
 * Revision 1.9  2010/02/21 19:50:06  clivewebster
 * Fix uartHasOverflow
 *
 * Revision 1.8  2010/02/09 15:00:37  clivewebster
 * Add uartGetReader and uartGetWriter for each uart
 *
 * Revision 1.7  2010/01/24 17:37:34  clivewebster
 * Add SendBuffer command
 *
 * Revision 1.6  2009/11/02 18:50:27  clivewebster
 * Added revision log
 *
 * ===========
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
 * _uart_common.h
 *
 *  Created on: 04-May-2009
 *      Author: Clive Webster
 *
 *
 *  Contains code common to all sorts of UARTs
 *
 *  These should not be called directly - they are called from hardware or
 *  software uart code
 *
 */

#ifndef _UART_COMMON_H_
#define _UART_COMMON_H_

#include "libdefs.h"
#include "errors.h"
#include "buffer.h"
#include "iopin.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// ------ Structures, macros ------------------------

// Default uart baud rate.
// This is the default speed after a uartInit() command,
// and can be changed by using uartSetBaudRate().
#ifndef UART_DEFAULT_BAUD_RATE
#define UART_DEFAULT_BAUD_RATE		((BAUD_RATE)9600)	///< default baud rate
#endif

typedef void (*voidFuncPtru08)(unsigned char, void*);

// Don't call these yourself - they are just for the library
extern void __uartSendBuffered(void* uart, uint8_t data);
extern void __uartSendUnbuffered(void* uart, uint8_t data);
extern void __uartSendNextBuffered(void* uart);
extern void __uartSendNextUnbuffered(void* uart);
extern void __uartReceiveBuffered(void* uart, uint8_t data);
extern void __uartReceiveUnbuffered(void* uart, uint8_t data);
extern int  __uartGetByteBuffered(void* uart);
extern int  __uartGetByteUnbuffered(void* uart);

// Fwd def
struct /*PROGMEM*/ c_uart;

typedef struct s_uart_commmon {
	cBuffer*  		 		rxBuffer;
	cBuffer*  		 		txBuffer;
	volatile voidFuncPtru08 rxFunc;			// function to call on receive char
	void*				rxFuncData;		// data to pass into rxFunc when called
	volatile boolean 	readyTx:1;
	volatile boolean 	sendingBuffer:1;	// Are we currently sending from the txBuffer
	volatile boolean    rxOverflow:1;
	const struct /*PROGMEM*/ c_uart*	classdef;
	volatile int		rxChar;				// a single byte receive queue
	const IOPin*		rx_pin;				// The receive pin
	const IOPin*		tx_pin;				// The xmit pin
	Reader				reader;				// The routine to read a byte from the uart
	Writer				writer;				// The routine to write a byte to the uart
	void*				cpp;				//  (in C++ this is the Uart instance)
	boolean				oneWire;			// Use a one wire interface - don't change it unless you know what you're doing!
	void (*xmitCallback)(void* param,boolean start);	// Call back for start/end receive
	void*				callbackParam;		// The parameter for the callback
	void (*tx)(void * uart,uint8_t data);	// The routine to send a byte or add it to the queue
	void (*txPut)(void * uart);				// The routine to put the next byte once the previous has done
	void (*rx)(void * uart,uint8_t data);	// The receive routine service
	int (*rxGet)(void * uart);				// The routine to return the next received character
} UART;

// Deprecated - causes compile issues with later versions of avr-gcc
// See the following 4 macros which should be used instead
/*
#define MAKE_UART_COMMON(rxBuf,txBuf,class, rxPin, txPin, reader, writer) { \
	rxBuf, txBuf, null, null, FALSE, FALSE, FALSE, class, -1, rxPin, txPin, reader, writer, null, FALSE,null,null,\
	(txBuf==null) ? &__uartSendUnbuffered : &__uartSendBuffered, \
	(txBuf==null) ? &__uartSendNextUnbuffered : &__uartSendNextBuffered, \
	(rxBuf==null) ? &__uartReceiveUnbuffered : &__uartReceiveBuffered, \
	(rxBuf==null) ? &__uartGetByteUnbuffered : &__uartGetByteBuffered \
	}
*/

// Create UART with a buffer on Rx and Tx
#define MAKE_UART_COMMON_Rx_Tx(rxBuf,txBuf,class, rxPin, txPin, reader, writer) { \
	rxBuf, txBuf, null, null, FALSE, FALSE, FALSE, class, -1, rxPin, txPin, reader, writer, null, FALSE,null,null,\
	&__uartSendBuffered   , &__uartSendNextBuffered, \
	&__uartReceiveBuffered, &__uartGetByteBuffered \
	}

// Create UART with a buffer on Rx but not on Tx
#define MAKE_UART_COMMON_Rx(rxBuf,class, rxPin, txPin, reader, writer) { \
	rxBuf, null, null, null, FALSE, FALSE, FALSE, class, -1, rxPin, txPin, reader, writer, null, FALSE,null,null,\
	&__uartSendUnbuffered , &__uartSendNextUnbuffered, \
	&__uartReceiveBuffered, &__uartGetByteBuffered \
	}

// Create UART with a buffer on Tx but not on Rx
#define MAKE_UART_COMMON_Tx(txBuf,class, rxPin, txPin, reader, writer) { \
	null, txBuf, null, null, FALSE, FALSE, FALSE, class, -1, rxPin, txPin, reader, writer, null, FALSE,null,null,\
	&__uartSendBuffered     , &__uartSendNextBuffered, \
	&__uartReceiveUnbuffered, &__uartGetByteUnbuffered \
	}

// Create UART with no buffers at all
#define MAKE_UART_COMMON(class, rxPin, txPin, reader, writer) { \
	null, null, null, null, FALSE, FALSE, FALSE, class, -1, rxPin, txPin, reader, writer, null, FALSE,null,null,\
	&__uartSendUnbuffered   , &__uartSendNextUnbuffered, \
	&__uartReceiveUnbuffered, &__uartGetByteUnbuffered \
	}



// Define the statics for a given uart class, stored in PROGMEM
typedef struct /*PROGMEM*/ c_uart{
	void     (*setBaudRate)(UART* uart, BAUD_RATE baudrate);	// Set the baud rate
	void     (*startXmit)(UART* uart, uint8_t data);			// Start sending one byte
	void     (*init)(UART* uart, BAUD_RATE baudrate);			// Initialise the UART
	void     (*off)(UART* uart);								// Turn off the uart
	void	 (*startXmitMode)(UART* uart);						// Start transmit mode
	void	 (*endXmitMode)(UART* uart);						// End transmit mode
} UART_CLASS;
#define MAKE_UART_CLASS(setBaud, startXmit, init, off, startXmitMode, endXmitMode) { \
	setBaud, startXmit, init, off, startXmitMode, endXmitMode }


// --- Public Functions ----

// Get the routine for writing to this uart
#define uartGetWriter(uart) __uartGetWriter(&((uart)->_uart_))
static __inline__ Writer __uartGetWriter(const UART* uart){
	return uart->writer;
}

// Get the routine for reading from this uart
#define uartGetReader(uart) __uartGetReader(&((uart)->_uart_))
static __inline__ Reader __uartGetReader(const UART* uart){
	return uart->reader;
}

// Returns pointer to the receive buffer structure.
#define uartGetRxBuffer(uart) __uartGetRxBuffer(&((uart)->_uart_))
static __inline__ cBuffer* __uartGetRxBuffer(const UART* uart){
	return uart->rxBuffer;
}

// Returns pointer to the transmit buffer structure.
#define uartGetTxBuffer(uart) __uartGetTxBuffer(&((uart)->_uart_))
static __inline__ cBuffer* __uartGetTxBuffer(const UART* uart){
	return uart->txBuffer;
}

/*
#define uartSetTransmitBuffer(uart,buffer) __uartSetTransmitBuffer(&((uart)->_uart_),buffer)
static __inline__ void __uartSetTransmitBuffer(UART* uart, cBuffer * buffer){
	CRITICAL_SECTION{
		uart->txBuffer = buffer;
		uart->tx = (buffer==null) ? &__uartSendUnbuffered : &__uartSendBuffered;
		uart->txPut = (buffer==null) ? &__uartSendNextUnbuffered : &__uartSendNextBuffered;
	}
}
*/

/*
#define uartSetReceiveBuffer(uart,buffer) __uartSetReceiveBuffer(&((uart)->_uart_),buffer)
static __inline__ void __uartSetReceiveBuffer(UART* uart, cBuffer* buffer){
	CRITICAL_SECTION{
		uart->rxBuffer = buffer;
		uart->rx = (buffer==null) ? &__uartReceiveUnbuffered : &__uartReceiveBuffered;
		uart->rxGet = (buffer==null) ? &__uartGetByteUnbuffered : &__uartGetByteBuffered;
	}
}
*/

// Has the receive buffer overflowed?
#define uartHasOverflowed(uart) __uartHasOverflowed(&((uart)->_uart_))
static __inline__ boolean __uartHasOverflowed(const UART* uart){
	return (uart->rxOverflow) ? TRUE : FALSE;
}

// Redirects received data to a user function.
#define uartAttach(uart,fn, device) __uartAttach(&((uart)->_uart_),fn, device)
static __inline__ void __uartAttach(UART* uart, void (*rx_func)(unsigned char, void*),void *device){
	CRITICAL_SECTION{
		uart->rxFunc = rx_func;
		uart->rxFuncData = device;
	}
}

#define uartDetach(uart) __uartDetach(&((uart)->_uart_))
static __inline__ void __uartDetach(UART* uart){
	__uartAttach(uart,null,null);
}

// Macro for backward compatibility
// deprectated - you should use uartAttach instead
#define uartSetRxHandler(uart,h) uartAttach(uart,h,null)

// Returns TRUE/FALSE if receive buffer is empty/not-empty.
#define uartReceiveBufferIsEmpty(uart) __uartReceiveBufferIsEmpty(&((uart)->_uart_))
boolean __uartReceiveBufferIsEmpty(const UART* uart);

// Flushes (deletes) all data from receive buffer.
#define uartFlushReceiveBuffer(uart) __uartFlushReceiveBuffer(&((uart)->_uart_))
void __uartFlushReceiveBuffer(UART* uart);

// Flushes (deletes) all data from transmit buffer.
#define uartFlushTransmitBuffer(uart) __uartFlushTransmitBuffer(&((uart)->_uart_))
static __inline__ void __uartFlushTransmitBuffer(UART* uart){
	bufferFlush(__uartGetTxBuffer(uart));
}

// Gets a single byte from the uart receive buffer - returns -1 if none
#define uartGetByte(uart) __uartGetByte(&((uart)->_uart_))
int __uartGetByte(UART* uart);

// Is the UART busy doing something?
#define uartIsBusy(uart) __uartIsBusy(&((uart)->_uart_))
static __inline__ boolean __uartIsBusy(UART* uart){
	return (uart->readyTx) ? FALSE : TRUE;
}

#define uartSetBaudRate(uart,baud) _uartSetBaudRate(&((uart)->_uart_),baud)
void _uartSetBaudRate(UART* uart, BAUD_RATE baud);

#define uartSendByte(uart,data) _uartSendByte(&((uart)->_uart_),data)
uint8_t _uartSendByte(UART* uart, uint8_t data);

#define uartSendBuffer(uart,data, count) _uartSendBuffer(&((uart)->_uart_),data,count)
void _uartSendBuffer(UART* uart, const uint8_t* data, size_t count);

#define uartInit(uart,baud) _uartInit(&((uart)->_uart_),baud)
void _uartInit(UART* uart, BAUD_RATE baud);

// turns off UART
#define uartOff(uart) _uartOff( &((uart)->_uart_) )
void _uartOff(UART* uart);

#define uartIsOneWire(uart) _uartIsOneWire( &((uart)->_uart_) )
static __inline__ boolean _uartIsOneWire(const UART* uart){
	return uart->oneWire;
}

//  ----- Private functions (not for use by your programs) -----
#define uartTransmitService(uart) __uartTransmitService(&(uart)->_uart_)
#define uartReceiveService(uart) __uartReceiveService(&(uart)->_uart_, _SFR_MEM8((uart)->data))


// A byte has been received
void __uartReceiveService(UART* uart, uint8_t c);
// We have finished transmitting the current byte
void __uartTransmitService(UART* uart);


#ifdef BUILDING_LIBRARY
// Routines that are private to the library
void _uartStartXmit(UART* uart,uint8_t txData);
void _uartTransmitBlockStart(UART* uart);
void _uartTransmitBlockEnd(UART* uart);
#endif


#ifdef __cplusplus
}
/* ===================== C++ Wrapper Code ===============================================*/
#include "Stream/Stream.h"
class Uart : public Stream {
public:
	Uart(UART * cstr): Stream(){
		m_cstr = cstr;
		cstr->cpp = this;
	}

	// Convert a C UART into a C++ Uart&
	static Uart& find(const UART* uart){
		Uart* inst = (Uart*)(uart->cpp);
		return *inst;
	}

	void on(BAUD_RATE baud) const{
		_uartInit(m_cstr, baud);
	}
	void off(void) const {
		_uartOff(m_cstr);
	}

	void attach(void (*rx_func)(unsigned char, Uart&, void*), void* param = NULL){
		m_rx_func = rx_func;						// Remember the user routine
		m_param = param;
		__uartAttach(m_cstr,&Uart::rx_func,this);	// Callback my static method

	}
	void detach(void){
		__uartDetach(m_cstr);
		m_rx_func = null;
	}

	boolean isBusy(void) const{
		return __uartIsBusy(m_cstr);
	}

	void flushRx(void) const{
		__uartFlushReceiveBuffer(m_cstr);
	}
	void flushTx(void) const{
		__uartFlushTransmitBuffer(m_cstr);
	}

	boolean isRxBufferEmpty(void) const{
		return __uartReceiveBufferIsEmpty(m_cstr);
	}

	operator UART*() const{
		return m_cstr;
	}

	/* Implement the raw methods from Stream */
	virtual int write(uint8_t data){
		return _uartSendByte(m_cstr, data);
	}
	virtual int read(void){
		return __uartGetByte(m_cstr);
	}

protected:
	// Generic call back
	static void rx_func(unsigned char c, void * device){
		Uart* inst = (Uart*)(device);
		if(inst->m_rx_func){
			inst->m_rx_func(c,*inst, inst->m_param);
		}
	}

	UART* m_cstr;

private:
	void (* m_rx_func)(unsigned char, Uart&, void* );
	void* m_param;
};
#endif

#endif /* _UART_COMMON_H_ */
