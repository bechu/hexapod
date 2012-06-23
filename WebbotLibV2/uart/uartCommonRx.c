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
 *        File: uartCommonRx.c
 *  Created on: 28 Dec 2010
 *      Author: Clive Webster
 */

#include "../_uart_common.h"



// A byte has been received - interrupts are off
void __uartReceiveService(UART* uart, uint8_t c){
	// if there's a user function to handle this receive event
	if(uart->rxFunc){
		// call it and pass the received data
		uart->rxFunc(c, uart->rxFuncData);
	}else{
		void (*fn)(void*, uint8_t) = uart->rx;
		fn(uart,c);				//	Call either the buffer or non buffered routine
	}
}


// Gets a single byte from the uart receive buffer.
// Returns the byte, or -1 if no byte is available (getchar-style).
int __uartGetByte(UART* uart){
	int (*fn)(void*) = uart->rxGet;
	return fn(uart);				//	Call either the buffer or non buffered routine
}

