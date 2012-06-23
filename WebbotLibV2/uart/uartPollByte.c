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
 *        File: uartPollByte.c
 *  Created on: 29 Dec 2010
 *      Author: Clive Webster
 */
#include "../uart.h"
#include <avr/io.h>

// compatibility for the mega161
#ifndef RXCIE
	#define RXCIE	RXCIE0
	#define TXCIE	TXCIE0
	#define RXEN	RXEN0
	#define TXEN	TXEN0
	#define RXC		RXC0
#endif

// Put the receiver in polling mode (TRUE) or interrupt mode (FALSE)
void uartReceivePollingMode(const HW_UART* uart,boolean polling){
	if(polling){
		// turn off receive interrupts
		_SFR_MEM8(uart->statusB) &=  ~(BV(RXCIE));		// Rx Off
	}else{
		// turn on receive interrupts
		_SFR_MEM8(uart->statusB) |=  BV(RXCIE);			// Rx On
	}
}


// Poll the hardware for the next receive character
int uartPollByte(const HW_UART* uart){
	int ch=-1;
	if( _SFR_MEM8(uart->statusA) & BV(RXC)){
		// A character is waiting
		ch = _SFR_MEM8(uart->data);

	}
	return ch;
}


