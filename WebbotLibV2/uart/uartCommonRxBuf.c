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
 *        File: uartCommonRxBuf.c
 *  Created on: 29 Jan 2011
 *      Author: Clive Webster
 */

#include "../_uart_common.h"

// Receive a character to a buffered UART
void __uartReceiveBuffered(void* _uart, uint8_t c){
	UART* uart=(UART*)_uart;
	// there is a receive buffer so try to append it
	if(bufferPut(__uartGetRxBuffer(uart),c)==FALSE){
		// track overflows
		uart->rxOverflow=TRUE;
		setError(UART_RECEIVE_OVERFLOW);
	}
}

int __uartGetByteBuffered(void* _uart){
	UART* uart=(UART*)_uart;

	// get single byte from receive buffer (if available)
	uint8_t c;
	int rtn;

	cBuffer* buffer = __uartGetRxBuffer(uart);
	// There is a receive buffer
	if(bufferGet(buffer,&c))
		rtn = c;
	else
		rtn = -1;
	return rtn;
}

