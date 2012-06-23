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
 *        File: _camera_yuv.c
 *  Created on: 9 Jun 2011
 *      Author: Clive Webster
 */

#include "_camera_common.h"

boolean _cameraSetBin_YUV(CAMERA* camera, uint8_t binNo, const COLOR* min, const COLOR* max){
	boolean rtn = FALSE;

	if(binNo < _cameraNumColorBins(camera)){
		CAMERA_BIN* bin = &camera->bins[binNo];

		COLOR _min;
		COLOR _max;
		COLOR_YUV *_minYUV;
		COLOR_YUV *_maxYUV;

		// Convert min to yuv
		_minYUV = color2yuv(min, &_min);

		// Convert max to yuv
		_maxYUV = color2yuv(max, &_max);

		if(_minYUV->y > _maxYUV->y){
			// swap the y values
			uint8_t c = _minYUV->y;
			_minYUV->y = _maxYUV->y;
			_maxYUV->y = c;
		}

		if(_minYUV->u > _maxYUV->u){
			// swap the u values
			uint8_t c = _minYUV->u;
			_minYUV->u = _maxYUV->u;
			_maxYUV->u = c;
		}

		if(_minYUV->v > _maxYUV->v){
			// swap the v values
			uint8_t c = _minYUV->v;
			_minYUV->v = _maxYUV->v;
			_maxYUV->v = c;
		}

		if(colorEquals(&_min, &bin->min)==FALSE || colorEquals(&_max, &bin->max)==FALSE){
			// The colours have changed

			// Put into color bank table
			color2yuv(&_min, &bin->min);
			color2yuv(&_max, &bin->max);
			bin->dirty = TRUE;
		}

		rtn = bin->active = TRUE;

	}
	return rtn;
}
