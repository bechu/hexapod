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
 *	Routines that are common to all transmit functions over a UART
 *
 *        File: uartCommonTx.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */
#include "../_uart_common.h"
#include "../core.h"

// Virutal function - start sending the given byte
void _uartStartXmit(UART* uart,uint8_t txData){
	const UART_CLASS* class = uart->classdef;
	void (*fn)(UART*,uint8_t) = (void (*)(UART*,uint8_t))pgm_read_word(&class->startXmit);
	fn(uart,txData);
}

// We are starting to transmit a block
void _uartTransmitBlockStart(UART* uart){
	uart->readyTx = FALSE;				// indicate we are sending
	if(uart->xmitCallback){
		uart->xmitCallback(uart->callbackParam,TRUE);
	}
	const UART_CLASS* class = uart->classdef;
	void (*fn)(UART*) = (void (*)(UART*))pgm_read_word(&class->startXmitMode);
	if(fn){
		fn(uart);
	}
}

// We have finished transmitting a block
void _uartTransmitBlockEnd(UART* uart){
	uart->readyTx = TRUE;				// indicate we are no longer sending
	const UART_CLASS* class = uart->classdef;
	void (*fn)(UART*) = (void (*)(UART*))pgm_read_word(&class->endXmitMode);
	if(fn){
		fn(uart);
	}
	if(uart->xmitCallback){
		uart->xmitCallback(uart->callbackParam,FALSE);
	}

}

// Send a block of data
void _uartSendBuffer(UART* uart, const uint8_t* data, size_t count){
	while(count--){
		_uartSendByte(uart,*data++);
	}
}


// Sends a single byte over the uart.
uint8_t _uartSendByte(UART* uart, uint8_t txData){
	void (*fn)(void*, uint8_t) = uart->tx;
	fn(uart,txData);	//	Call either the buffer or non buffered routine
	return txData;
}




// We have finished transmitting the current byte
void __uartTransmitService(UART* uart){
	void (*fn)(void*) = uart->txPut;
	fn(uart);					//	Call either the buffer or non buffered routine
}

