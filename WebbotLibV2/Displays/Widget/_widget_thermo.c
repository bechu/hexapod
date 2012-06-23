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
 *        File: _widget_thermo.c
 *  Created on: 21 Jul 2011
 *      Author: Clive Webster
 */

#include "../_gdisplay_common.h"
#include "../../core.h"
#include <math.h>

static PIXEL getRadius(const THERMO_WIDGET* dial){
	PIXEL sz = MIN(dial->_widget_.w,dial->_widget_.h);
	sz >>= 1;
	sz -= dial->_widget_.padding;
	sz >>= 1;
	if(sz <= 2){
		sz = 2;
	}
	return sz;
}


static void draw(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	THERMO_WIDGET* dial = (THERMO_WIDGET*)widget;

	// Get the radius of the bulb
	PIXEL radius = getRadius(dial);
	PIXEL width = 2 * radius / 3;
	PIXEL bulbx = (dial->_widget_.w >> 1) + dial->_widget_.x;
	PIXEL bulby = dial->_widget_.y + dial->_widget_.h - dial->_widget_.padding - radius;
	PIXEL topy = dial->_widget_.y + dial->_widget_.padding;

	// Draw border of vertical square
	_displaySetInk(display,&dial->_props_.border);
	_displayMoveTo(display, bulbx, bulby);
	_displayRectangleAt(display, bulbx-width, bulby, bulbx+width, topy, FALSE);

	// Create the dial background circle
	_displayMoveTo(display, bulbx, bulby);
	_displaySetInk(display,&dial->_props_.border);
	_displayCircle(display, radius, TRUE);
	_displaySetInk(display,&dial->_widget_.content);
	_displayCircle(display, radius-1, TRUE);

	// Put grad marks for every x
	if(dial->_props_.gradVal != 0){
		_displaySetInk(display, &dial->_props_.grad);
		PIXEL maxx = bulbx-width+1;
		PIXEL minx = bulbx-radius;
		PIXEL miny = bulby-radius;
		PIXEL maxy = topy + 1;
		for(double grad = dial->_props_.setting.min; grad <= dial->_props_.setting.max; grad+=dial->_props_.gradVal){
			double bar = interpolatef(grad,
					dial->_props_.setting.min, dial->_props_.setting.max,
					miny,maxy);
			PIXEL bary = round(bar);
			_displayMoveTo(display, minx, bary);
			_displayLineTo(display, maxx, bary);
		}
	}

}



// Redraw the active part of the widget
static void update(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	THERMO_WIDGET* dial = (THERMO_WIDGET*)widget;

	// Get the radius of the bulb
	PIXEL radius = getRadius(dial);
	PIXEL bulby = dial->_widget_.y + dial->_widget_.h - dial->_widget_.padding - radius;
	PIXEL topy = dial->_widget_.y + dial->_widget_.padding;

	PIXEL miny = bulby-radius;
	PIXEL maxy = topy + 1;

	PIXEL bary = round(interpolatef(dial->_props_.setting.value,
			dial->_props_.setting.min, dial->_props_.setting.max,
			miny,maxy));
	PIXEL oldbary = round(interpolatef(dial->_props_.setting.old,
			dial->_props_.setting.min, dial->_props_.setting.max,
			miny,maxy));

	if(oldbary != bary){
		PIXEL width = 2 * radius / 3;
		PIXEL bulbx = (dial->_widget_.w >> 1) + dial->_widget_.x;
		PIXEL minx = bulbx-width+1;
		PIXEL maxx = bulbx+width-1;

		if(bary != miny){
			// Plot mercury
			_displaySetInk(display,&dial->_widget_.content);
			_displayRectangleAt(display, minx, miny, maxx, bary, TRUE);
		}
		if(bary != maxy){
			// Blank out
			_displaySetInk(display,&dial->_props_.border);
			_displayRectangleAt(display, minx, bary, maxx, maxy, TRUE);
		}
	}
}

void thermoSetValue(THERMO_WIDGET* dial, double val){
	dial->_widget_.dirty |= _widgetSetValue(&dial->_props_.setting, val);
}


const GDISPLAY_WIDGET_CLASS PROGMEM c_thermo_widget = MAKE_GDISPLAY_WIDGET_CLASS(&draw,&update);
