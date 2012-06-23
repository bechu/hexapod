
/*
 * $Id: _camera_common.c,v 1.2 2010/06/14 18:41:37 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _camera_common.c,v $
 * Revision 1.2  2010/06/14 18:41:37  clivewebster
 * Add copyright license info
 *
 * Revision 1.1  2010/02/04 22:28:35  clivewebster
 * Added common camera API
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
 * _camera_common.c
 *
 *  Created on: 31-Jan-2010
 *      Author: Clive Webster
 *
 *  Redirect calls to the correct virtual methods for a given camera
 */
#include "_camera_common.h"
#include <string.h>
#include <stdlib.h>

void     _cameraInit(CAMERA* camera){

	// Make RAM space for the colour bins
	if(camera->bins == NULL){
		camera->bins = malloc( _cameraNumColorBins(camera) * sizeof(CAMERA_BIN));
	}

	// Zap all colour bins to RGB(0,0,0) except bin#0
	if(camera->bins != NULL){
		uint8_t maxBins = _cameraNumColorBins(camera);
		for(uint8_t i = 0; i < maxBins; i++){
			CAMERA_BIN* bin = &camera->bins[i];
			bin->active = bin->dirty = FALSE;
			if(i==0){
				COLOR min,max;
				colorSetRGB(&min,180,180,180);
				colorSetRGB(&max,255,255,255);
				_cameraSetBin(camera,i,&min,&max);
			}else{
				colorSetRGB(&(bin->min),0,0,0);
				colorSetRGB(&(bin->max),0,0,0);
			}
		}
	}

	// Allocate space for blobs
	if(camera->blobs == null){
		camera->blobs = malloc(_cameraMaxBlobs(camera) * sizeof(CAMERA_BLOB));
	}


	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	void (*fn)(CAMERA*) = (void (*)(CAMERA*))pgm_read_word(&class->init);
	if(fn){
		fn(camera);
	}
}

uint16_t  _cameraXresolution(CAMERA* camera){
	uint16_t rtn = 0;
	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	uint16_t (*fn)(CAMERA*) = (uint16_t (*)(CAMERA*))pgm_read_word(&class->xResolution);
	if(fn){
		rtn = fn(camera);
	}
	return rtn;
}

uint16_t  _cameraYresolution(CAMERA* camera){
	uint16_t rtn = 0;
	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	uint16_t (*fn)(CAMERA*) = (uint16_t (*)(CAMERA*))pgm_read_word(&class->yResolution);
	if(fn){
		rtn = fn(camera);
	}
	return rtn;
}

uint8_t  _cameraNumColorBins(const CAMERA* camera){
	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	return (uint8_t)pgm_read_byte(&class->numColorBins);
}

char*  _cameraGetVersion(CAMERA* camera){
	char* rtn = null;
	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	char* (*fn)(CAMERA*) = (char* (*)(CAMERA*))pgm_read_word(&class->getVersion);
	if(fn){
		rtn = fn(camera);
	}
	return rtn;
}

uint8_t  _cameraMaxBlobs(const CAMERA* camera){
	return 8;
}

// Set the value of a colour bin to RGB
boolean _cameraSetBin(CAMERA* camera, uint8_t binNo, const COLOR* min, const COLOR* max){
	boolean rtn = FALSE;

	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	boolean (*fn)(CAMERA*, uint8_t,const COLOR*,const COLOR*) = (boolean (*)(CAMERA*, uint8_t,const COLOR*,const COLOR*))pgm_read_word(&class->setBin);
	if(fn){
		rtn = fn(camera,binNo,min,max);
	}
	return rtn;
}

uint8_t   _cameraGetBlobs(CAMERA* camera,uint8_t bin){
	uint16_t rtn = 0;
	camera->numBlobs = 0;

	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	uint8_t (*fn)(CAMERA*, uint8_t) = (uint8_t (*)(CAMERA*, uint8_t))pgm_read_word(&class->getBlobs);
	if(fn){
		rtn = fn(camera,bin);
	}
	return rtn;
}

boolean _cameraGetPixel(CAMERA* camera,uint16_t x, uint16_t y, COLOR * color){
	boolean rtn = FALSE;

	// Get the method implementation for this class of camera
	const CAMERA_CLASS* class = camera->clazz;
	boolean (*fn)(CAMERA*,uint16_t,uint16_t,COLOR*) = (boolean (*)(CAMERA*, uint16_t,uint16_t,COLOR*))
		pgm_read_word(&class->getPixel);
	if(fn){
		rtn = fn(camera,x,y,color);
	}
	return rtn;

}



// Library only method to insert a new blob
// This uses an insert sort
void	_cameraInsertBlob(CAMERA* camera, const CAMERA_BLOB* blob){
	uint16_t width = blob->right - blob->left + 1;
	uint16_t height = blob->bottom - blob->top + 1;
	uint32_t pixels = width * height;
	if(pixels >= camera->minBlobSize && camera->blobs != null){
		// Its big enough to be considered
		uint8_t maxBlobs = _cameraMaxBlobs(camera);

		uint8_t slot;
		// Find the position to insert it
		for(slot = 0; slot < camera->numBlobs; slot++){
			if(camera->blobs[slot].pixels < pixels){
				// insert it here
				uint8_t last = MIN(camera->numBlobs, maxBlobs -1);
				while( last < slot+1){
					memcpy(&camera->blobs[last], &camera->blobs[last-1], sizeof(CAMERA_BLOB));
				}
				break;
			}
		}

		// set variables at blob[slot]
		if(slot < maxBlobs){
			CAMERA_BLOB* dest = &camera->blobs[slot];
			memcpy(dest, blob, sizeof(CAMERA_BLOB));
			dest->pixels = pixels;
			dest->xCenter = (width / 2) + dest->left - (_cameraXresolution(camera) / 2);
			dest->yCenter = (height / 2) + dest->top - (_cameraYresolution(camera) / 2);
			dest->yCenter *= -1; // Invert the y axis

			if(camera->numBlobs < maxBlobs){
				camera->numBlobs++;
			}
		}
	}
}



const CAMERA_BLOB* _cameraFetchBlob(const CAMERA* camera, uint8_t blobNo){
	return &camera->blobs[blobNo];
}

