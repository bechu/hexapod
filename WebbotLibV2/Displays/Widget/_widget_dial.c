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
 *        File: _widget_dial.c
 *  Created on: 18 Jul 2011
 *      Author: Clive Webster
 */

#include "../_gdisplay_common.h"
#include <string.h>
#include <math.h>
#include "../../Maths/Vector2D.h"
#include "../../Maths/Matrix2D.h"
#include "../../core.h"

static PIXEL getRadius(const DIAL_WIDGET* dial){
	PIXEL sz = MIN(dial->_widget_.w,dial->_widget_.h);
	sz /= 2;
	sz -= dial->_widget_.padding;
	if(sz <= 0){
		sz = 1;
	}
	return sz;
}

static PIXEL centerX(const DIAL_WIDGET* dial){
	PIXEL radius = getRadius(dial);
	PIXEL space = dial->_widget_.w - 2 * radius ;
	space /= 2;
	return dial->_widget_.x + space + radius;
}

static PIXEL centerY(const DIAL_WIDGET* dial){
	PIXEL radius = getRadius(dial);
	PIXEL space = dial->_widget_.h - 2 * radius ;
	space /= 2;
	return dial->_widget_.y + space + radius;
}

static void draw(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	DIAL_WIDGET* dial = (DIAL_WIDGET*)widget;

	// Get the center of the dial
	PIXEL radius = getRadius(dial);
	//PIXEL innerRadius = radius * (100 - dial->dialThickness) / 100;
	VECTOR2D north = MAKE_VECTOR2D(0,-1);
	VECTOR2D origin = MAKE_VECTOR2D(centerX(dial),centerY(dial));
	MATRIX2D rotate;

	// Create the dial background circle
	_displayMoveTo(display, origin.x, origin.y);
	_displaySetInk(display,&dial->_props_.dial);
	_displayCircle(display, radius, TRUE);
//	_displaySetInk(display,&dial->_widget_.bgnd);
//	_displayCircle(display, innerRadius, TRUE);


	// Put grad marks for every x
	if(dial->_props_.gradVal != 0){
		_displaySetInk(display, &dial->_props_.grad);
		for(double grad = dial->_props_.setting.min; grad <= dial->_props_.setting.max; grad+=dial->_props_.gradVal){

			// Create Rotation matrix
			double angle = interpolatef(grad, dial->_props_.setting.min, dial->_props_.setting.max, dial->_props_.minAng, dial->_props_.maxAng);
			double rad = angle * M_PI/180.0;
			matrix2d_SetRotate(&rotate, rad);

			// Rotate the North vector
			VECTOR2D max,min;
			matrix2d_Transform(&max, &north, &rotate);
			vector2d_Copy(&min,&max);
			vector2d_Scale(&max,radius);
			double scale = 0.85;
			vector2d_Scale(&min,radius * scale);
			vector2d_Add(&min,&origin);
			vector2d_Add(&max,&origin);


			// Draw line from min to max
			_displayMoveTo(display, round(min.x), round(min.y));
			_displayLineTo(display, round(max.x), round(max.y));
		}
	}
}


// Redraw the active part of the widget
static void update(GDISPLAY_WIDGET* widget){
	GDISPLAY* display = widget->_owner_;
	DIAL_WIDGET* dial = (DIAL_WIDGET*)widget;


	VECTOR2D north = MAKE_VECTOR2D(0,getRadius(dial) * -0.8);
	VECTOR2D west = MAKE_VECTOR2D(-3,0);

	// Create the rotation matrix for the current value
	MATRIX2D rotate;
	double angle = interpolatef(dial->_props_.setting.value,
			dial->_props_.setting.min, dial->_props_.setting.max,
			dial->_props_.minAng, dial->_props_.maxAng);
	matrix2d_SetRotate(&rotate, angle * M_PI/180.0);

	// Rotate the North vector to find the new tip
	VECTOR2D tip;
	matrix2d_Transform(&tip,   &north, &rotate);
	vector2d_Round(&tip, &tip);


	// Create the rotation matrix for the previous value
	MATRIX2D rotateOld;
	double angleOld = interpolatef(dial->_props_.setting.old,
			dial->_props_.setting.min, dial->_props_.setting.max,
			dial->_props_.minAng, dial->_props_.maxAng);
	matrix2d_SetRotate(&rotateOld, angleOld * M_PI/180.0);

	// Rotate the North vector to find the old tip
	VECTOR2D oldtip;
	matrix2d_Transform(&oldtip,   &north, &rotateOld);
	vector2d_Round(&oldtip,&oldtip);


	// Rotate the West Vector to find the needle start left and right
	VECTOR2D left,right;
	matrix2d_Transform(&left , &west, &rotate);
	right.x = - left.x;
	right.y = - left.y;
	vector2d_Round(&left, &left);
	vector2d_Round(&right, &right);

	// Rotate the West Vector to find the needle start left and right
	VECTOR2D oldleft,oldright;
	matrix2d_Transform(&oldleft , &west, &rotateOld);
	oldright.x = - oldleft.x;
	oldright.y = - oldleft.y;
	vector2d_Round(&oldleft, &oldleft);
	vector2d_Round(&oldright, &oldright);


	// Define the origin
	VECTOR2D origin = MAKE_VECTOR2D(centerX(dial),centerY(dial));


	// Make everything relative to the origin
	vector2d_Add(&tip,&origin);
	vector2d_Add(&left,&origin);
	vector2d_Add(&right,&origin);

	vector2d_Add(&oldtip,&origin);
	vector2d_Add(&oldleft,&origin);
	vector2d_Add(&oldright,&origin);

	// Replace the variable content colour with the background colour
	_displaySetInk(display,&dial->_props_.dial);
	_displayTriangleAt(display, oldtip.x,oldtip.y, oldleft.x,oldleft.y, oldright.x,oldright.y,TRUE);

	// Draw it
	_displaySetInk(display,&dial->_widget_.content);
	_displayTriangleAt(display, tip.x,tip.y, left.x,left.y, right.x,right.y,TRUE);
}

void dialSetValue(DIAL_WIDGET* dial, double val){
	dial->_widget_.dirty |= _widgetSetValue(&dial->_props_.setting, val);
}



const GDISPLAY_WIDGET_CLASS PROGMEM c_dial_widget = MAKE_GDISPLAY_WIDGET_CLASS(&draw,&update);
