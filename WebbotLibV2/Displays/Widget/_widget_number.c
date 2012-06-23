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
 *        File: _widget_number.c
 *  Created on: 20 Jul 2011
 *      Author: Clive Webster
 */


#include "../_gdisplay_common.h"
#include <stdlib.h>

#include "../../segled.h"

void numberSetValue(NUMBER_WIDGET* numWidget, double val){
	numWidget->_widget_.dirty |= _widgetSetValue(&numWidget->_props_.setting, val);
}

static void draw(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	NUMBER_WIDGET* numb = (NUMBER_WIDGET*)widget;

	// Draw rectangle around digits
	_displaySetInk(display, &numb->_props_.dial);
	_displayRectangleAt(display,
			numb->_widget_.x + numb->_widget_.padding , numb->_widget_.y + numb->_widget_.padding,
			numb->_widget_.x + numb->_widget_.w - numb->_widget_.padding, numb->_widget_.y + numb->_widget_.h - numb->_widget_.padding,
			TRUE);
	numb->_props_.first = TRUE;
}

// Line width
static void drawH(GDISPLAY* display, PIXEL w){
	PIXEL LW = w / 8;
	if(w > 0){

		_displayMoveBy(display,1,0);		// left hand gap

		_displayTriangleBy(display, 0,0, LW,LW, LW,-LW, TRUE );
		_displayMoveBy(display,LW,0);		// move over triangle

		_displayRectangleBy(display, 0, -LW, w - (1+LW+LW+1), LW, TRUE);
		_displayMoveBy(display,w - (1+LW+LW+1),0);		// move over rectangle

		_displayTriangleBy(display, 0,-LW, 0,LW, LW,0, TRUE );
		_displayMoveBy(display,LW,0);		// move over triangle

		_displayMoveBy(display,1,0);		// right hand gap
	}else{
		_displayMoveBy(display, w, 0);		// Move to LHS
		drawH(display,-w);					// Draw to RHS
		_displayMoveBy(display, w, 0);		// Move to LHS
	}
}
static void drawV(GDISPLAY* display, PIXEL h){
	PIXEL LW = h / 8;
	if(h > 0){
		_displayMoveBy(display,0,1);		// top gap

		_displayTriangleBy(display, 0,0, -LW,LW, LW,LW, TRUE );
		_displayMoveBy(display,0, LW);		// move over triangle

		_displayRectangleBy(display, -LW, 0, LW,h - (1+LW+LW+1), TRUE);
		_displayMoveBy(display,0,h - (1+LW+LW+1));		// move over rectangle

		_displayTriangleBy(display, -LW,0, 0,LW, LW,0, TRUE );
		_displayMoveBy(display,0,LW);		// move over triangle

		_displayMoveBy(display,0,1);		// bottom gap
	}else{
		_displayMoveBy(display, 0, h);		// Move to top
		drawV(display,-h);					// Draw to bottom
		_displayMoveBy(display, 0, h);		// Move to top
	}
}
static void moveH(GDISPLAY* display, PIXEL w){
	_displayMoveBy(display,w,0);
}
static void moveV(GDISPLAY* display, PIXEL h){
	_displayMoveBy(display,0,h);
}

static void drawMask(GDISPLAY* display,PIXEL fw,PIXEL fh,uint8_t mask, const COLOR* color){
	_displaySetInk(display,color);

/*
	PIXEL oldx = display->gx;
	PIXEL oldy = display->gy;
*/
	// Segment A
	if(mask & 0b10000000){
		drawH(display,fw);
	}else{
		moveH(display,fw);
	}

	// Segment B
	if(mask & 0b01000000){
		drawV(display,fh);
	}else{
		moveV(display,fh);
	}

	// Segment G
	if(mask & 0b00000010){
		drawH(display,-fw);
		moveH(display,fw);
	}

	// Segment C
	if(mask & 0b00100000){
		drawV(display,fh);
	}else{
		moveV(display,fh);
	}

	// Segment D
	if(mask & 0b00010000){
		drawH(display,-fw);
	}else{
		moveH(display,-fw);
	}

	// Segment E
	if(mask & 0b00001000){
		drawV(display,-fh);
	}else{
		moveV(display,-fh);
	}

	// Segment F
	if(mask & 0b00000100){
		drawV(display,-fh);
	}else{
		moveV(display,-fh);
	}

	// We are now back to the top left corner
	// Segment H = full stop
	if(mask & 0b00000001){
		PIXEL LW = fw / 4;
		if(LW<1) LW=1;

		PIXEL x,y;
		x = fw + LW * 2;
		y = fh*2 - LW;

		_displayMoveBy(display, x,y);
		_displayCircle(display, LW,TRUE);
		_displayMoveBy(display, -x, -y);
	}

/*
	PIXEL nowx = display->gx;
	PIXEL nowy = display->gy;

	if(nowx != oldx){
		setError(1);
	}
	if(nowy != oldy){
		setError(2);
	}
*/
}

static void putDigit(GDISPLAY_WIDGET* widget, PIXEL fontWidth, PIXEL fontHeight, char c, boolean dot, char old, boolean olddot){
	NUMBER_WIDGET* numb = (NUMBER_WIDGET*)widget;
	GDISPLAY* display = widget->_owner_;

	// Get the mask of things to show
	uint8_t mask = segled8_get_mask(c);
	if(dot){
		mask |= 1;
	}

	// Get the mask of old things on show
	uint8_t oldmask;

	if(numb->_props_.first){
		oldmask = 0;
	}else{
		oldmask = segled8_get_mask(old);
		if(olddot){
			oldmask |= 1;
		}
	}

	// Find which segments have changed state
	uint8_t changes;
	changes = mask ^ oldmask;

	if( changes !=0 ){
		// Something has changed
		PIXEL fh = fontHeight / 2;
		PIXEL fw = fontWidth * 5 / 8;

		// Get the segments to remove
		uint8_t remove = oldmask & changes;
//		remove = 0xff;	// remove all
		if(remove){
			drawMask(display,fw,fh,remove,&numb->_props_.dial);
		}

		// Get the segments to add
		uint8_t add = mask & changes;
//		add = mask;	// add all
		if(add){
			drawMask(display,fw,fh,add,&numb->_widget_.content);
		}
	}
}

#define FONT_VPAD 2

static void update(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	NUMBER_WIDGET* numb = (NUMBER_WIDGET*)widget;
	char txt[20];
	char oldtxt[20];

	signed char width = numb->_props_.numDigits;
	if(numb->_props_.setting.min < 0){
		width++;
	}
	unsigned char prec = numb->_props_.numDecimals;

	signed char dWidth = width;
	if(prec>0){
		dWidth += prec + 1;
		width += prec;
	}

	// Convert to text
	dtostrf(numb->_props_.setting.value, dWidth, prec, txt );
	dtostrf(numb->_props_.setting.old,   dWidth, prec, oldtxt );

	// Replace the variable content colour with the background colour
//	_displayReplace(display, &widget->content, &numb->dial, widget->x, widget->y, widget->w, widget->h);

	// The display width of the dial
	PIXEL dispHeight = (widget->h - widget->padding - widget->padding);
	PIXEL dispWidth = widget->w - widget->padding - widget->padding;

	// Get the bounding box of each character
	PIXEL fontHeight = dispHeight - (FONT_VPAD*2);
	// Make enough space for bottom bar
	fontHeight -= fontHeight / 8;

	PIXEL fontWidth  = dispWidth / (width+1);		// Leave half a char to the left and right

	PIXEL fontPadLeft = (dispWidth - (fontWidth * width))/2;

	// Move to top left of display area
	_displayMoveTo(display, fontPadLeft + numb->_widget_.x + numb->_widget_.padding , FONT_VPAD + numb->_widget_.y + numb->_widget_.padding);

	// Now lets show each digit
	char* digit = txt;
	char* old = oldtxt;
	while(*digit){
		if(*digit != '.'){
			if(numb->_props_.first || *digit != *old){
				putDigit(widget, fontWidth, fontHeight,
						*digit, (digit[1]=='.') ? TRUE : FALSE,
						*old, (old[1]=='.') ? TRUE : FALSE
						);
			}
			// Move to next char along
			_displayMoveBy(display, fontWidth, 0);
		}
		digit++;
		old++;
	}
	numb->_props_.first = FALSE;
}


const GDISPLAY_WIDGET_CLASS PROGMEM c_number_widget = MAKE_GDISPLAY_WIDGET_CLASS(&draw,&update);
