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
 *        File: uartCommonRxNoBuf.c
 *  Created on: 29 Jan 2011
 *      Author: Clive Webster
 */

#include "../_uart_common.h"

// Receive a character to a non buffered UART
void __uartReceiveUnbuffered(void* _uart, uint8_t c){
	UART* uart=(UART*)_uart;
	if(uart->rxChar == -1){
		uart->rxChar = c;
	}else{
		uart->rxOverflow=TRUE;
		setError(UART_RECEIVE_OVERFLOW);
	}
}

int __uartGetByteUnbuffered(void* _uart){
	UART* uart=(UART*)_uart;

	// get single byte from receive buffer (if available)
	int rtn;

	CRITICAL_SECTION{
		rtn = uart->rxChar;		// get the last received char
		uart->rxChar = -1;		// and reset it
	}
	return rtn;
}

