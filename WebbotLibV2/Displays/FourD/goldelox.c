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
 *        File: goldelox.c
 *  Created on: 15 Jul 2011
 *      Author: Clive Webster
 */

#include "ser4D.h"
#include "../../clock.h"
#include "../../core.h"
#include <stdlib.h>

#define ACK 6
#define NAK 0x15

#define AUTOBAUD 	0x55
#define INFO_CMD	0x56
#define CLS_CMD		0x45
#define FONT_CMD	0x46
#define BKGND_CMD   0x4B
#define LINE_CMD   	0x4C
#define PEN_CMD   	0x70
#define TRIANGLE_CMD 0x47
#define CIRCLE_CMD  0x43
#define RECTANGLE_CMD  0x72
#define CUT_PASTE_CMD 0x63
#define REPLACE_CMD 0x6b
#define DRAW_PIXEL_CMD 0x50
#define READ_PIXEL_CMD 0x52
#define TRANSPARENT_CMD 0x4f

#define TIMEOUT_DELAY 1000000	/* 1 second */

#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})


static GENERIC_4D_DISPLAY* toDisplay(GDISPLAY* g){
	GENERIC_4D_DISPLAY* rtn = container_of(g, GENERIC_4D_DISPLAY, _gdisplay_ );
	return rtn;
}


// The data returned by the INFO_CMD
typedef struct s_info{
	uint8_t	deviceType;
	uint8_t	hardwareRev;
	uint8_t	firmwareRev;
	uint8_t	hres;
	uint8_t	vres;
} INFO;

static boolean pluggedIn(const GENERIC_4D_DISPLAY* device){
	return pin_is_high(device->reset);
}

static void discard(unsigned char c, void* device){
}

// Get the response byte
// Return:
//   +1 if ACK
//    0 if NAK
//   -1 if timed out
static int8_t getAckNakResponse(const DISPLAY* display){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	Reader r = device->_uart_->reader;

	// Wait till everything sent
	while(__uartIsBusy(device->_uart_)){
		breathe();
	}

	TICK_COUNT start = clockGetus();
	for(;;){
		int ch = r();
		if(ch!=-1){
			uint8_t c = ch & 0xFF;
			if( c==ACK){
				return 1;
			}
			if( c==NAK){
				return 0;
			}
		}else{
			// No response so far
			if(clockHasElapsed(start,TIMEOUT_DELAY)){
				break;
			}
			breathe();
		}

	}
	return -1;
}

/*
 * Get a response of a given size
 * Return FALSE if timed out, TRUE if success
 */
static boolean getResponse(GENERIC_4D_DISPLAY* device, void* dta, size_t numBytes){
	Reader r = device->_uart_->reader;

	// Wait till everything sent
	while(__uartIsBusy(device->_uart_)){
		breathe();
	}

	if(!pluggedIn(device)){
		return FALSE;
	}

	TICK_COUNT start = clockGetus();
	uint8_t* dest = (uint8_t*)dta;
	uint8_t index;

	for(index = 0; index < numBytes;){
		int ch = r();
		if(ch!=-1){
			uint8_t c = ch & 0xFF;
			dest[index++] = c;
		}else{
			// No response so far
			if(clockHasElapsed(start,TIMEOUT_DELAY)){
				return FALSE;
			}
			breathe();
		}
	}
	return TRUE;
}


// Write a raw value to the screen
static void sendRawByte(const DISPLAY* display, uint8_t c){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	if(device->pending){
		getAckNakResponse(display);
		device->pending = FALSE;
	}
	device->_uart_->writer(c);
}

static void sendPixelPosition(GENERIC_4D_DISPLAY* device, PIXEL val){
	if(device->picaso){
		// Put msb
		sendRawByte(&device->_display_, (val >> 8));
	}
	sendRawByte(&device->_display_, (val & 255));
}

static void sendColor(GENERIC_4D_DISPLAY* device, const COLOR_RGB* rgb){
	// msb = r4r3r2r1r0g5g4g3
	uint8_t msb = (rgb->r & 0xf8) | (rgb->g  >> 5);
	// lsb = g2g1g0b4b3b2b1b0
	uint8_t lsb = ( (rgb->g << 3) & 0xe0) | ( rgb->b >> 3);

	sendRawByte(&device->_display_, msb);
	sendRawByte(&device->_display_, lsb);
}

static void sendFgndColor(GENERIC_4D_DISPLAY* device){
	COLOR_RGB* rgb = color2rgb( &device->_gdisplay_.fgnd, &device->_gdisplay_.fgnd);
	sendColor(device, rgb);
}

static void sendBgndColor(GENERIC_4D_DISPLAY* device){
	COLOR_RGB* rgb = color2rgb( &device->_gdisplay_.bgnd, &device->_gdisplay_.bgnd);
	sendColor(device, rgb);
}


static PIXEL drawChar(GDISPLAY* gdisplay, uint8_t c, PIXEL x, PIXEL y, boolean transparent){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;

	if(pluggedIn(device)){
		_displaySetTransparent(display,transparent);
		if( c < 0x20){
			// Draw the custom character
			sendRawByte(display, 0x44);	// draw custom character in graphics format
			if(device->picaso){
				sendRawByte(display, 0);// use group 0 ie 8*8 fonts
			}
			sendRawByte(display, c);	// the character to draw
			sendPixelPosition(device,x);
			sendPixelPosition(device,y);
			sendFgndColor(device);
		}else{
			// Draw the ascii character
			sendRawByte(display, 0x74);	// draw ascii character in graphics format
			sendRawByte(display, c);	// the character to draw
			sendPixelPosition(device,x);
			sendPixelPosition(device,y);
			sendFgndColor(device);
			sendRawByte(display, 1);	// scale width by 1
			sendRawByte(display, 1);	// scale height by 1
		}
		device->pending = TRUE;
	}
	return device->fontWidth;
}

// Write a text character to the screen
// This will use the 'Draw as Graphic' command
// This is called when writing to the screen in 'text only' mode
static void sendChar(const DISPLAY* display, uint8_t c){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	PIXEL x = device->_display_.cursorX * device->fontWidth;
	PIXEL y = device->_display_.cursorY * device->fontHeight;
	drawChar(&device->_gdisplay_, c, x, y, FALSE);
}

// Convert a 4D Systems res constant to number of pixels - see data sheet
static PIXEL resolution(uint8_t val){
	switch(val){
	case 0x22: return 220;
	case 0x28: return 128;
	case 0x32: return 320;
	case 0x60: return 160;
	case 0x64: return 64;
	case 0x76: return 176;
	case 0x96: return 196;
	}
	return 1;	// unknown
}

/*
 * Get info about the current device
 * And then update the number of rows/columns etc
 */
static void getInfo(GENERIC_4D_DISPLAY* device){
	if(pluggedIn(device)){
		// Request the info about the current device
		sendRawByte(&device->_display_, INFO_CMD);
		sendRawByte(&device->_display_, 0);

		INFO result;
		if(getResponse(device, &result, sizeof(INFO))){
			// Get the x,y resolutions
			device->_gdisplay_.gxRes = resolution(result.hres);
			device->_gdisplay_.gyRes = resolution(result.vres);
			device->firmware = result.firmwareRev;
			device->hardware = result.hardwareRev;
			device->type = result.deviceType;
		}
	}
}

// Set the font to the closest approximation to the given size
// Available fonts:
//			GOLDELOX	PICASO		Cols	Rows
//			5 x 7		5 x 7		21		16
//			8 x 8		8 x 8		16		16
//			8 x 12		8 x 12		16		10
//						12 x 16
static void setFont(GENERIC_4D_DISPLAY* device, FONT_4D font, uint8_t hpad, uint8_t vpad){
	DISPLAY* display = &device->_display_;

	uint8_t w,h;
	switch(font){
	case FONT_8_8:
		w = h = 8;
		break;
	case FONT_8_12:
		w = 8; h = 12;
		break;
	case FONT_12_16:
		if(device->picaso){
			w = 12; h = 16;
			break;
		} // else fall through to...
	default:
		font = FONT_5_7;
		w = 5; h = 7;
		break;
	}
	w += hpad;
	h += vpad;

	sendRawByte(display,FONT_CMD);
	sendRawByte(display,font);
	if(getAckNakResponse(display)== 1){
		// Font has been ack'ed

		device->fontWidth = w;
		device->fontHeight = h;

		// Calc number of rows and columns
		PIXEL cols = device->_gdisplay_.gxRes / w;
		PIXEL rows = device->_gdisplay_.gyRes / h;

		// Put it in the range 1 to 255
		cols = CLAMP(cols, 1, 255);
		rows = CLAMP(rows, 1, 255);

		// Set the display size in text units
		device->_display_.columns = cols;
		device->_display_.rows = rows;

		// Re-allocate the buffer
		device->_display_.buffer = realloc(device->_display_.buffer, rows * cols);

		// Reset the cursor position so that x,y are valid
		displayGoto(device,device->_display_.cursorX,device->_display_.cursorY);
	}
}


/*
 * According to datasheet
 * The device may send garbage whilst booting up
 * Need to set Tx output pin high as soon as possible to stop misunderstanding of start bit
 * Wait for 500ms + time for the SD card which is at least 75ms
 * Send autobaud character 'U' until ACK received
 */
static void init(DISPLAY* display){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	// bgnd = black
	colorSetRGB(&device->_gdisplay_.bgnd, 0,0,0 );
	// fgnd = white
	colorSetRGB(&device->_gdisplay_.fgnd, 255,255,255 );

	// Attach routine for dumping received chars
	__uartAttach(device->_uart_,&discard, device);

	// Initialise the uart
	// For the picaso we need to start in 9600 baud
	_uartInit(device->_uart_ ,(device->picaso) ? (BAUD_RATE)9600 : device->baudrate);

	// Reset the device using a brief 'low' pulse
	// Note the reset pin is 'open collector'
	pin_make_output(device->reset,FALSE);
	clockWaitus(500);
	pin_make_input(device->reset, FALSE);  // Allow LCD pullup to set the right voltage

	// Now wait for at least 1 second from reset
	TICK_COUNT start = clockGetus();
	while(!clockHasElapsed(start,1000000)){
		breathe();
	}

	// If the display is not there then do nothing
	if(!pluggedIn(device)){
		return;
	}

	// Start listening again
	__uartDetach(device->_uart_);
	__uartFlushReceiveBuffer(device->_uart_);

	// Send the baudrate command and wait for an ACK
	int8_t response;
	do{
		sendRawByte(display, AUTOBAUD);
		response = getAckNakResponse(display);
	}while(response != 1);

	// for the picaso you can now change to the required baud rate
	if(device->picaso){
		BAUD_RATE reqd = device->baudrate;
		uint8_t byt;
		if(reqd == 19200){
			byt = 8;
		}else if(reqd == 38400){
			byt = 0x0a;
		}else if(reqd == 57600){
			byt = 0x0c;
		}else if(reqd == 115200){
			byt = 0x0d;
		}else if(reqd == 128000){
			byt = 0x0e;
		}else if(reqd == 256000){
			byt = 0x0f;
		}else{
			// continue at 9600
			byt = 6;
		}

		if(byt != 6){
			// Send command using old baud rate
			sendRawByte(display, 0x51);	// change baud rate
			sendRawByte(display, byt);	// to the new rate

			// Now change processor to use new baud rate
			_uartInit(device->_uart_ ,device->baudrate);
			// Expect ack/nak response
			device->pending = TRUE;
		}
	}

	// Now get info on the device
	getInfo(device);

	// set the font to 5 x 7
	setFont(device, FONT_5_7, 2, 1);
//	setFont(device, FONT_8_12);

	// Set text to opaque so that when it over-prints it uses the background colour
	_displaySetTransparent(display,TRUE);
}

static void cls(DISPLAY* display){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	// Set current position to 0,0
	device->_gdisplay_.gx = device->_gdisplay_.gy = 0;

	if(pluggedIn(device)){
		// Send cmd to clear the screen to the current background colour
		sendRawByte(display, CLS_CMD);
		device->pending = TRUE;
	}

}

static void home(DISPLAY* display){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	// Home the current graphic co-ord
	device->_gdisplay_.gx = device->_gdisplay_.gy = 0;
}

// Background colour has been updated
static void setBgnd(GDISPLAY* gdisplay){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		sendRawByte(display, BKGND_CMD);
		sendBgndColor(device);
		device->pending = TRUE;
	}
}

// Set contrast to a value constrained from 0 to 100
static void contrast(DISPLAY* display, uint8_t percent){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	if(pluggedIn(device)){
		// Convert contrast to 0...15
		uint16_t temp = percent;
		temp *= 15;
		temp /= 100;

		sendRawByte(display, 0x59);
		sendRawByte(display, 2);
		sendRawByte(display, temp);
		device->pending = TRUE;
	}
}

static void backlight(DISPLAY* display,boolean on){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	if(device->picaso && pluggedIn(device)){
		sendRawByte(display, 0x59);
		sendRawByte(display, 0);
		sendRawByte(display, (on) ? 1 : 0);
		device->pending = TRUE;
	}
}


static void shutdown(GDISPLAY* gdisplay){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;

	// Clear to black
	displaySetPaperRGB(device,0,0,0);
	displayClear(device);

	// Turn contrast off
	displayContrast(device,0);

	// Turn backlight off
	displayBacklight(device,FALSE);

	// Turn display off
	if(pluggedIn(device)){
		sendRawByte(display, 0x59);
		sendRawByte(display, 3);
		sendRawByte(display, 0);
		getAckNakResponse(display);
	}

}

static void line(GDISPLAY* gdisplay, PIXEL x, PIXEL y){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		sendRawByte(display, LINE_CMD);
		sendPixelPosition(device,gdisplay->gx);
		sendPixelPosition(device,gdisplay->gy);
		sendPixelPosition(device,x);
		sendPixelPosition(device,y);
		sendFgndColor(device);
		device->pending = TRUE;
	}
}

static void plot(GDISPLAY* gdisplay, PIXEL x, PIXEL y){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		sendRawByte(display, DRAW_PIXEL_CMD);
		sendPixelPosition(device,x);
		sendPixelPosition(device,y);
		sendFgndColor(device);
		device->pending = TRUE;
	}
}

static boolean read(GDISPLAY* gdisplay, PIXEL x, PIXEL y, COLOR* color){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	boolean ok = FALSE;
	if(pluggedIn(device)){
		sendRawByte(display, READ_PIXEL_CMD);
		sendPixelPosition(device,x);
		sendPixelPosition(device,y);
		// get returned two bytes
		uint8_t rtn[2];
		if(getResponse(device, &rtn, sizeof(rtn))){
			// Get the x,y resolutions
			uint8_t r,g,b;
			r = rtn[0] >> 3;
			r <<= 3;

			g = rtn[1] >> 5;
			g |= (rtn[0] & 7) << 3;
			g <<= 2;

			b = rtn[1] & 0x1f;
			b <<= 3;

			if(color){
				colorSetRGB(color, r,g,b);
			}

			ok = TRUE;
		}
	}
	return ok;
}


static void setFill(GENERIC_4D_DISPLAY* device, boolean fill){
	if(device->fill != fill){
		DISPLAY* display = &device->_display_;
		device->fill = fill;
		if(pluggedIn(device)){
			sendRawByte(display, PEN_CMD);
			sendRawByte(display, (fill) ? 0 : 1);
			device->pending = TRUE;
		}
	}
}

static void triangle(GDISPLAY* gdisplay, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, PIXEL x3, PIXEL y3, boolean fill){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		setFill(device,fill);

		// Check listed in anti-clockwise order
		// Vector 1 is from x1,y1 to x2,y2
		PIXEL v1x = x2 - x1;
		PIXEL v1y = y2 - y1;
		// Vector 2 is from x1,y1 to x3,y3
		PIXEL v2x = x3 - x1;
		PIXEL v2y = y3 - y1;

		// Get z of cross product of the two vectors
		int32_t z = (v1x * v2y) - (v1y * v2x);
		if(z > 0){
			// swap x2,y2 and x3,y3
			PIXEL t;
			t = x2; x2=x3; x3=t;
			t = y2; y2=y3; y3=t;
		}

		sendRawByte(display, TRIANGLE_CMD);
		sendPixelPosition(device,x1);
		sendPixelPosition(device,y1);
		sendPixelPosition(device,x2);
		sendPixelPosition(device,y2);
		sendPixelPosition(device,x3);
		sendPixelPosition(device,y3);
		sendFgndColor(device);
		device->pending = TRUE;
	}
}
static void replace(GDISPLAY* gdisplay, const COLOR *src, const COLOR *dst, PIXEL x, PIXEL y, PIXEL w, PIXEL h){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	if(pluggedIn(device)){
		DISPLAY* display = &device->_display_;

		COLOR old,new;
		COLOR_RGB* rgbSrc = color2rgb(src, &old);
		COLOR_RGB* rgbDst = color2rgb(dst, &new);

		sendRawByte(display, REPLACE_CMD);
		sendPixelPosition(device,x);
		sendPixelPosition(device,y);
		sendPixelPosition(device,x + w - 1);
		sendPixelPosition(device,y + h - 1);
		sendColor(device, rgbSrc);
		sendColor(device, rgbDst);
		device->pending = TRUE;
	}
}

static void rectangle(GDISPLAY* gdisplay, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		setFill(device,fill);

		sendRawByte(display, RECTANGLE_CMD);
		sendPixelPosition(device,x1);
		sendPixelPosition(device,y1);
		sendPixelPosition(device,x2);
		sendPixelPosition(device,y2);
		sendFgndColor(device);
		device->pending = TRUE;
	}
}

static void rectangleClear(GDISPLAY* gdisplay, PIXEL x1, PIXEL y1, PIXEL x2, PIXEL y2, boolean fill){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(pluggedIn(device)){
		setFill(device,fill);

		sendRawByte(display, RECTANGLE_CMD);
		sendPixelPosition(device,x1);
		sendPixelPosition(device,y1);
		sendPixelPosition(device,x2);
		sendPixelPosition(device,y2);
		sendBgndColor(device);
		device->pending = TRUE;
	}
}

static void circle(GDISPLAY* gdisplay, PIXEL radius, boolean fill){
	GENERIC_4D_DISPLAY* device = toDisplay(gdisplay);
	DISPLAY* display = &device->_display_;
	if(radius && pluggedIn(device)){
		setFill(device,fill);

		sendRawByte(display, CIRCLE_CMD);
		sendPixelPosition(device,gdisplay->gx);
		sendPixelPosition(device,gdisplay->gy);
		sendPixelPosition(device,radius);
		sendFgndColor(device);
		device->pending = TRUE;
	}
}

// Program an 8*8 custom character
static void custom(DISPLAY* display,  const uint8_t* pgm_def, uint8_t charNum){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	if(pluggedIn(device)){
		sendRawByte(display, 0x41);
		if(device->picaso){
			sendRawByte(display, 0);// use group 0 ie 8*8 fonts
		}
		sendRawByte(display, charNum);
		for(uint8_t i=0; i<8; i++){
			sendRawByte(display, pgm_read_byte(&pgm_def[i]));
		}
		device->pending = TRUE;
	}
}


static void hgraph(DISPLAY* display,uint16_t pixels,uint16_t max, uint8_t width){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	PIXEL yTop    = device->_display_.cursorY * device->fontHeight;
	PIXEL yBottom = yTop + device->fontHeight - 1;

	PIXEL xLeft  = device->_display_.cursorX * device->fontWidth;
	PIXEL xRight = xLeft + width * device->fontWidth - 1;
	PIXEL xBar = interpolateU(pixels, 0,max, xLeft,xRight);

	if(xBar != xLeft){
		// Draw bar in foreground colour
		rectangle(&device->_gdisplay_, xLeft, yTop, xBar, yBottom, TRUE);
	}
	if(xBar != xRight){
		// Blank remaining
		rectangleClear(&device->_gdisplay_, xBar, yTop, xRight, yBottom, TRUE);
	}

}

// Scroll the display up one line
/*
static void scrollUp(DISPLAY* display){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	if(pluggedIn(device)){
		sendRawByte(display, CUT_PASTE_CMD);
		sendPixelPosition(device, 0);							// Src Top Left X
		sendPixelPosition(device, device->fontHeight);			// Src Top Left Y
		sendPixelPosition(device, 0);							// Dst Top Left X
		sendPixelPosition(device, 0);							// Dst Top Left Y
		sendPixelPosition(device, device->_gdisplay_.gxRes);	// Width
		sendPixelPosition(device, (device->_display_.rows-1)*device->fontHeight);// Height
		getAckNakResponse(display);


		// Blank the last line
		rectangleClear( &device->_gdisplay_,
				0,
				(device->_display_.rows-1)*device->fontHeight,
				device->_gdisplay_.gxRes-1,
				device->_gdisplay_.gyRes-1,
				TRUE);
	}
}
*/

static void transparent(DISPLAY* display,boolean t){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;
	if(display->transparent != t && pluggedIn(device)){
		sendRawByte(display,TRANSPARENT_CMD);
		sendRawByte(display, (t) ? 0 : 1);
		device->pending = TRUE;
	}
}

static void vgraph(DISPLAY* display,DISPLAY_COLUMN x,DISPLAY_COLUMN y, uint16_t pixels,uint16_t max,uint8_t height){
	GENERIC_4D_DISPLAY* device = (GENERIC_4D_DISPLAY*)display;

	PIXEL yTop    = device->_display_.cursorY * device->fontHeight;
	PIXEL yBottom = yTop + height * device->fontHeight - 1;

	PIXEL xLeft  = device->_display_.cursorX * device->fontWidth;
	PIXEL xRight = xLeft + device->fontWidth - 1;

	PIXEL yBar = interpolateU(pixels, max,0, yTop, yBottom);

	if(yBar != yBottom){
		// Draw bar in foreground colour
		rectangle(&device->_gdisplay_, xLeft, yBottom, xRight, yBar, TRUE);
	}
	if(yBar != yTop){
		// Blank remaining
		rectangleClear(&device->_gdisplay_, xLeft, yBar, xRight, yTop, TRUE);
	}


}

const DISPLAY_CLASS PROGMEM c_tGOLDELOX = MAKE_DISPLAY_CLASS( \
		&init, /* init */ \
		&cls, /* cls */ \
		&home, /* home */ \
		null, /* gotoXY = no hardware interaction required as there is no cursor*/ \
		null, /* lineWrap = no hardware interaction required*/ \
		null, /* autoScroll = no hardware interaction required*/ \
		&backlight, /* backlight = GOLDELOX has no backlight control */ \
		null, /* setBrightness = GOLDELOX has no brightness control */ \
		&contrast, /* setContrast */ \
		&sendChar, \
		&hgraph, /* hgraph */ \
		&vgraph, /* vgraph */ \
		&custom,  /* custom */ \
		null,		  /* scrollUp */ \
		&transparent  /* transparency */ \
);


const GDISPLAY_CLASS PROGMEM c_gGOLDELOX = MAKE_GDISPLAY_CLASS( \
		&shutdown,	/* shutdown 		*/	\
		null,		/* set fgnd colour 	*/	\
		&setBgnd,	/* set bgnd colour 	*/	\
		&line,		/* draw line		*/	\
		&triangle,	/* draw triangle	*/  \
		&circle,	/* draw circle		*/  \
		&rectangle,	/* draw rectangle	*/  \
		&replace,	/* color replace	*/  \
		&plot,		/* plot pixel		*/  \
		&read,		/* read pixel		*/  \
		&drawChar	/* draw char at x,y */  \
);

