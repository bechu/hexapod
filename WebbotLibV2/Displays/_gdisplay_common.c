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
 *        File: _gdisplay_common.c
 *  Created on: 15 Jul 2011
 *      Author: Clive Webster
 */

#include "_gdisplay_common.h"

void _displayShutdown(GDISPLAY* display){
	const GDISPLAY_CLASS* class = display->gclass;
	void (*fn)(GDISPLAY*) = (void (*)(GDISPLAY*))pgm_read_word(&class->shutdown);
	if(fn){
		fn(display);
	}
}

void _displaySetPaper(GDISPLAY* display, const COLOR* color){
	// Store it
	color2rgb(color,&display->bgnd);

	// Notify device driver
	const GDISPLAY_CLASS* class = display->gclass;
	void (*fn)(GDISPLAY*) = (void (*)(GDISPLAY*))pgm_read_word(&class->bkgnd);
	if(fn){
		fn(display);
	}
}

void _displaySetPaperRGB(GDISPLAY* display, uint8_t r, uint8_t g, uint8_t b){
	COLOR c;
	colorSetRGB(&c,r,g,b);
	_displaySetPaper(display, &c);
}

void _displaySetInk(GDISPLAY* display, const COLOR* color){
	// Store it
	color2rgb(color,&display->fgnd);

	// Notify device driver
	const GDISPLAY_CLASS* class = display->gclass;
	void (*fn)(GDISPLAY*) = (void (*)(GDISPLAY*))pgm_read_word(&class->fgnd);
	if(fn){
		fn(display);
	}
}

void _displaySetInkRGB(GDISPLAY* display, uint8_t r, uint8_t g, uint8_t b){
	COLOR c;
	colorSetRGB(&c,r,g,b);
	_displaySetInk(display, &c);
}

void _displayMoveTo(GDISPLAY* display, PIXEL x, PIXEL y){
	display->gx = x;
	display->gy = y;
}

void _displayMoveBy(GDISPLAY* display, PIXEL x, PIXEL y){
	display->gx += x;
	display->gy += y;
}

void _displayLineTo(GDISPLAY* display, PIXEL x, PIXEL y){
	const GDISPLAY_CLASS* class = display->gclass;
	void (*fn)(GDISPLAY*, PIXEL, PIXEL) = (void (*)(GDISPLAY*, PIXEL, PIXEL))pgm_read_word(&class->line);
	if(fn){
		fn(display, x,y);
	}
	_displayMoveTo(display, x, y);
}

void _displayLineBy(GDISPLAY* display, PIXEL x, PIXEL y){
	_displayLineTo(display, display->gx + x, display->gy + y);
}


// Leaves graphic cursor at x1,y1
void _displayTriangleAt(GDISPLAY* display, PIXEL x1,PIXEL y1, PIXEL x2,PIXEL y2, PIXEL x3,PIXEL y3, boolean fill){
	const GDISPLAY_CLASS* class = display->gclass;

	_displayMoveTo(display, x1, y1);

	void (*fn)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, boolean) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, boolean))pgm_read_word(&class->triangle);
	if(fn){
		fn(display, x1, y1, x2, y2, x3,y3, fill);
	}
}

// Leaves graphic cursor unchanged
// Co-ordinates are relative to current position
void _displayTriangleBy(GDISPLAY* display, PIXEL x1,PIXEL y1, PIXEL x2,PIXEL y2, PIXEL x3,PIXEL y3, boolean fill){
	const GDISPLAY_CLASS* class = display->gclass;

	void (*fn)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, boolean) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, PIXEL, boolean))pgm_read_word(&class->triangle);
	if(fn){
		fn(display, x1+display->gx, y1+display->gy, x2+display->gx, y2+display->gy, x3+display->gx,y3+display->gy, fill);
	}
}

// Leaves graphic cursor at x1,y1
void _displayRectangleAt(GDISPLAY* display, PIXEL x1,PIXEL y1, PIXEL x2,PIXEL y2, boolean fill){
	const GDISPLAY_CLASS* class = display->gclass;

	_displayMoveTo(display, x1, y1);

	void (*fn)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, boolean) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, boolean))pgm_read_word(&class->rectangle);
	if(fn){
		fn(display, x1, y1, x2, y2, fill);
	}
}

// Leaves graphic cursor unchanged
// Co-ordinates are relative to current position
void _displayRectangleBy(GDISPLAY* display, PIXEL x1,PIXEL y1, PIXEL x2,PIXEL y2, boolean fill){
	const GDISPLAY_CLASS* class = display->gclass;

	void (*fn)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, boolean) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL, PIXEL, PIXEL, boolean))pgm_read_word(&class->rectangle);
	if(fn){
		fn(display, x1+display->gx, y1+display->gy, x2+display->gx, y2+display->gy, fill);
	}
}

// Leaves graphic cursor at x,y
void _displayPlotAt(GDISPLAY* display, PIXEL x,PIXEL y){
	const GDISPLAY_CLASS* class = display->gclass;

	_displayMoveTo(display, x, y);

	void (*fn)(GDISPLAY*, PIXEL, PIXEL) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL))pgm_read_word(&class->plot);
	if(fn){
		fn(display, x, y);
	}
}

// Leaves graphic cursor unchanged
// Co-ordinates are relative to current position
void _displayPlotBy(GDISPLAY* display, PIXEL x,PIXEL y){
	const GDISPLAY_CLASS* class = display->gclass;

	void (*fn)(GDISPLAY*, PIXEL, PIXEL) =
			(void (*)(GDISPLAY*, PIXEL, PIXEL))pgm_read_word(&class->plot);
	if(fn){
		fn(display, x+display->gx, y+display->gy);
	}
}

// Leaves graphic cursor at x,y
boolean _displayReadAt(GDISPLAY* display, PIXEL x,PIXEL y, COLOR* color){
	const GDISPLAY_CLASS* class = display->gclass;
	boolean rtn = FALSE;

	_displayMoveTo(display, x, y);

	boolean (*fn)(GDISPLAY*, PIXEL, PIXEL, COLOR*) =
			(boolean (*)(GDISPLAY*, PIXEL, PIXEL, COLOR*))pgm_read_word(&class->read);
	if(fn){
		rtn = fn(display, x, y, color);
	}
	return rtn;
}

// Leaves graphic cursor unchanged
// Co-ordinates are relative to current position
boolean _displayReadBy(GDISPLAY* display, PIXEL x,PIXEL y, COLOR* color){
	const GDISPLAY_CLASS* class = display->gclass;
	boolean rtn = FALSE;

	boolean (*fn)(GDISPLAY*, PIXEL, PIXEL, COLOR*) =
			(boolean (*)(GDISPLAY*, PIXEL, PIXEL, COLOR*))pgm_read_word(&class->read);
	if(fn){
		rtn = fn(display, x+display->gx, y+display->gy, color);
	}
	return rtn;
}

// Draw circle at current graphics co-ord, leaves it at current co-ord
void _displayCircle(GDISPLAY* display, PIXEL radius, boolean fill){
	const GDISPLAY_CLASS* class = display->gclass;

	void (*fn)(GDISPLAY*, PIXEL, boolean) =
			(void (*)(GDISPLAY*, PIXEL, boolean))pgm_read_word(&class->circle);
	if(fn){
		fn(display, radius, fill);
	}
}

// Replace color 'src' with color 'dst' in the given rectangle
void _displayReplace(GDISPLAY* display, const COLOR *src, const COLOR *dst, PIXEL x, PIXEL y, PIXEL w, PIXEL h){
	const GDISPLAY_CLASS* class = display->gclass;

	void (*fn)(GDISPLAY*, const COLOR *,const COLOR *, PIXEL, PIXEL,PIXEL,PIXEL) =
			(void (*)(GDISPLAY*, const COLOR *,const COLOR *, PIXEL, PIXEL,PIXEL,PIXEL))pgm_read_word(&class->replace);
	if(fn){
		fn(display, src, dst, x,y,w,h);
	}

}

// Draw char at current location and move current location to be after char
void _displayChar(GDISPLAY* display, uint8_t c, boolean transparent){
	const GDISPLAY_CLASS* class = display->gclass;

	PIXEL (*fn)(GDISPLAY*, uint8_t, PIXEL,PIXEL,boolean) =
			(PIXEL (*)(GDISPLAY*, uint8_t, PIXEL,PIXEL,boolean))pgm_read_word(&class->drawChar);
	if(fn){
		display->gx += fn(display, c, display->gx, display->gy,transparent);
	}
}

