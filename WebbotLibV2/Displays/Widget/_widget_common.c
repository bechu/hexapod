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
 *        File: _widget_common.c
 *  Created on: 18 Jul 2011
 *      Author: Clive Webster
 */
#include "../_gdisplay_common.h"
#include <string.h>

void _widgetDraw(GDISPLAY_WIDGET* widget){
	const GDISPLAY_WIDGET_CLASS* class = widget->clazz;
	GDISPLAY* display = widget->_owner_;

	// Backup the current fgnd color
	COLOR old;
	memcpy(&old, &display->fgnd, sizeof(COLOR));

	// Clear whole widget to background colour
	_displaySetInk(display, &widget->bgnd);
	_displayRectangleAt(display, widget->x, widget->y, widget->x + widget->w, widget->y + widget->h,TRUE);

	// Do any other drawing required by the widget
	void (*fn)(GDISPLAY_WIDGET*) = (void (*)(GDISPLAY_WIDGET*))pgm_read_word(&class->draw);
	if(fn){
		fn(widget);
	}

	// Restore the current fgnd color
	memcpy(&display->fgnd, &old, sizeof(COLOR));

	// Force the variable content to be draw as well
	widget->dirty = TRUE;
}


// Redraw the contents of the widget if it is dirty
void _widgetUpdate(GDISPLAY_WIDGET* widget){

	if(widget->dirty){
		const GDISPLAY_WIDGET_CLASS* class = widget->clazz;
		GDISPLAY* display = widget->_owner_;

		// Backup the current fgnd color
		COLOR old;
		memcpy(&old, &display->fgnd, sizeof(COLOR));

		_displaySetInk(display, &widget->content);

		void (*fn)(GDISPLAY_WIDGET*) = (void (*)(GDISPLAY_WIDGET*))pgm_read_word(&class->update);
		if(fn){
			fn(widget);
		}

		// Restore the current fgnd color
		memcpy(&display->fgnd, &old, sizeof(COLOR));

		widget->dirty = FALSE;

	}
}

// Change a variable value
boolean _widgetSetValue(WIDGET_VALUE* var, double val){
	// Limit value to be in range
	val = CLAMP(val,var->min,var->max);

	// Update the value of the variable
	if(val != var->value){
		var->old = var->value;
		var->value = val;
		return TRUE;
	}
	return FALSE;
}
