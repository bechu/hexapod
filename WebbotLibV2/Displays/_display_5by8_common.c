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
 *        File: _display_5by8_common.c
 *  Created on: 17 Jul 2011
 *      Author: Clive Webster
 *
 *  Add Graph support for common 5x8 font character displays
 *
 */
#include "_display_common.h"

// Custom chars are 5 pixels across and 8 pixels down
static const uint8_t PROGMEM HorizCustomChar[] = {
		// Horizontal bar graph
		0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, // 0. 1/5 full progress block
		0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // 1. 2/5 full progress block
		0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, // 2. 3/5 full progress block
		0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00, // 3. 4/5 full progress block
		0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // 4. 5/5 full progress block
};

// Vertical bar graph
static const uint8_t PROGMEM VertCustomChar[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E
};

void setHGraph(DISPLAY* display){
	if(display->graph != 1){
		for(uint8_t c = 0; c<5; c++){
			_displayCustomChar(display, &HorizCustomChar[c<<3], c);
		}

		display->graph = 1;
	}
}

void setVGraph(DISPLAY* display){
	if(display->graph != 2){
		// Set up vertical graph chars
		for(uint8_t c=0; c<8; c++){
			_displayCustomChar(display, &VertCustomChar[c], c);
		}
		display->graph = 2;
	}
}

