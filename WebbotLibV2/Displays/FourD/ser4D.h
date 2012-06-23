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
 *        File: ser4D.h
 *  Created on: 15 Jul 2011
 *      Author: Clive Webster
 */

#ifndef SER4D_H_
#define SER4D_H_

#include "../../uart.h"
#include "../../iopin.h"
#include "../_gdisplay_common.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

typedef enum {
	OLED_4D = 0,
	LCD_4D = 1,
	VGA_4D = 2
} TYPE_4D;

typedef enum {
	FONT_5_7 = 0,
	FONT_8_8 = 1,
	FONT_8_12 = 2,
	FONT_12_16 = 3	/* PICASO ONLY - ON GOLDELOX use 5_7 and multiply by 2*/
} FONT_4D;

typedef struct s_4d_display {
	DISPLAY _display_;			// Common to all displays (must be first thing)
	GDISPLAY _gdisplay_;		// Common to all graphic displays
	UART*	_uart_;
	BAUD_RATE baudrate;
	const IOPin* reset;			// The output pin to reset the device
	boolean		picaso;			// TRUE if PICASO, FALSE if GOLDELOX
	boolean	    fill;			// TRUE if we are in fill mode (ie pensize=0)
	boolean	   pending;			// TRUE if an ACK/NAK response is pending
	uint8_t    hardware;		// hardware revision
	uint8_t    firmware;		// firmware revision
	TYPE_4D	   type;			// device type
	uint8_t	   fontWidth;		// Font Width including padding
	uint8_t	   fontHeight;		// Font Height including padding
} GENERIC_4D_DISPLAY;

extern const DISPLAY_CLASS c_tGOLDELOX;
extern const GDISPLAY_CLASS c_gGOLDELOX;

#define MAKE_GOLDELOX(uart,baud,reset,writer) { \
	MAKE_DISPLAY(&c_tGOLDELOX, 1, 1, writer), \
	MAKE_GDISPLAY(&c_gGOLDELOX), \
	&((uart)->_uart_), \
	baud, reset, FALSE, FALSE, FALSE,	\
	0,0 \
}

#define MAKE_PICASO(uart,baud,reset,writer) { \
	MAKE_DISPLAY(&c_tGOLDELOX, 1, 1, writer), \
	MAKE_GDISPLAY(&c_gGOLDELOX), \
	&((uart)->_uart_), \
	baud, reset, TRUE, FALSE, FALSE,	\
	0,0 \
}

#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/

class Generic4Ddisplay : public GraphicDisplay{
public:
	Generic4Ddisplay(GENERIC_4D_DISPLAY* cstr) : GraphicDisplay(&cstr->_gdisplay_, &cstr->_display_){

	}
};
#endif

#endif /* SER4D_H_ */
