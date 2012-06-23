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
 *        File: uartCommonTxBuf.c
 *  Created on: 29 Jan 2011
 *      Author: Clive Webster
 */

#include "../_uart_common.h"
#include "../core.h"

// Send a character via a buffered UART
void __uartSendBuffered(void* _uart, uint8_t data){
	UART* uart=(UART*)_uart;

	// Wait if the buffer is full
	while(bufferIsFull(uart->txBuffer)){
		breathe();
	}

	CRITICAL_SECTION{
		if(!uart->sendingBuffer){
			// Not already transmitting so start sending the buffer
			uart->sendingBuffer = TRUE;			// we are sending a buffer
			_uartTransmitBlockStart(uart);		// indicate we are sending
			_uartStartXmit(uart,data);			// put the byte
		}else{
			// add char to the output queue that is already being sent
			bufferPut(uart->txBuffer,data);
		}
	}

}

// Called when the previous byte has been sent
void __uartSendNextBuffered(void* _uart){
	UART* uart=(UART*)_uart;

	// there is a transmit buffer
	uint8_t c;

	CRITICAL_SECTION{
		if(bufferGet(__uartGetTxBuffer(uart),&c)){
			// send the next byte
			_uartStartXmit(uart,c);
		}else{
			// all done
			_uartTransmitBlockEnd(uart);		// we have finished sending
			uart->sendingBuffer = FALSE;		// we are not sending a buffer
		}
	}
}

