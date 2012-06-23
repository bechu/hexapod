
/*
 * $Id:  $
 *
 * Revision History
 * ================
 * $Log:
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (Webbot)
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
 * _camera_rgb.c
 *
 *  Created on: 09-Jun-2011
 *      Author: Clive Webster
 *
 */

#include "_camera_common.h"

boolean _cameraSetBin_RGB(CAMERA* camera, uint8_t binNo, const COLOR* min, const COLOR* max){
	boolean rtn = FALSE;

	if(binNo < _cameraNumColorBins(camera)){
		CAMERA_BIN* bin = &camera->bins[binNo];

		COLOR _min;
		COLOR _max;
		COLOR_RGB *_minRGB;
		COLOR_RGB *_maxRGB;

		// Convert min to rgb
		_minRGB = color2rgb(min, &_min);

		// Convert max to rgb
		_maxRGB = color2rgb(max, &_max);

		if(_minRGB->r > _maxRGB->r){
			// swap the red values
			uint8_t c = _minRGB->r;
			_minRGB->r = _maxRGB->r;
			_maxRGB->r = c;
		}

		if(_minRGB->g > _maxRGB->g){
			// swap the green values
			uint8_t c = _minRGB->g;
			_minRGB->g = _maxRGB->g;
			_maxRGB->g = c;
		}

		if(_minRGB->b > _maxRGB->b){
			// swap the blue values
			uint8_t c = _minRGB->b;
			_minRGB->b = _maxRGB->b;
			_maxRGB->b = c;
		}

		if(colorEquals(&_min, &bin->min)==FALSE || colorEquals(&_max, &bin->max)==FALSE){
			// The colours have changed

			// Put into color bank table
			color2rgb(&_min, &bin->min);
			color2rgb(&_max, &bin->max);
			bin->dirty = TRUE;
		}

		rtn = bin->active = TRUE;

	}
	return rtn;
}
