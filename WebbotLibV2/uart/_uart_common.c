/*
 * $Id: _uart_common.c,v 1.11 2010/06/14 19:21:25 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _uart_common.c,v $
 * Revision 1.11  2010/06/14 19:21:25  clivewebster
 * Add copyright license info
 *
 * Revision 1.10  2010/05/09 22:07:06  clivewebster
 * *** empty log message ***
 *
 * Revision 1.9  2010/04/25 03:33:56  clivewebster
 * Handle receive buffer calls better when there is no buffer
 *
 * Revision 1.8  2010/03/24 19:48:54  clivewebster
 * *** empty log message ***
 *
 * Revision 1.7  2010/03/07 19:31:50  clivewebster
 * *** empty log message ***
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
 * _uart_common.c
 *
 *  Created on: 04-May-2009
 *      Author: Clive Webster
 */

#include "../_uart_common.h"

void _uartSetBaudRate(UART* uart, BAUD_RATE baud){
	const UART_CLASS* class = uart->classdef;
	void (*fn)(UART*,BAUD_RATE) = (void (*)(UART*,BAUD_RATE))pgm_read_word(&class->setBaudRate);
	fn(uart,baud);
}



void _uartInit(UART* uart, BAUD_RATE baud){
	CRITICAL_SECTION{
		uart->oneWire = (uart->rx_pin == uart->tx_pin) ? TRUE : FALSE;
		// For one wire Make the pins into inputs with pullups enabled
		if(_uartIsOneWire(uart)){
			if(uart->rx_pin != uart->tx_pin){
				pin_make_input(uart->rx_pin,FALSE);
			}
			pin_make_input(uart->tx_pin,TRUE);
		}

		// Initialise local variable
		uart->readyTx = TRUE;
		uart->rxOverflow = FALSE;
		uart->sendingBuffer = FALSE;
	//	uart->rxFunc = null;

		const UART_CLASS* class = uart->classdef;
		void (*fn)(UART*,BAUD_RATE) = (void (*)(UART*,BAUD_RATE))pgm_read_word(&class->init);

		fn(uart,baud);
	}
}

