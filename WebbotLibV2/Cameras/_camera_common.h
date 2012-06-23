/*
 * $Id: _camera_common.h,v 1.3 2010/06/14 18:41:37 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _camera_common.h,v $
 * Revision 1.3  2010/06/14 18:41:37  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/02/09 14:59:25  clivewebster
 * Changed comment
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
 *
 * _camera_common.h
 *
 *  Created on: 31-Jan-2010
 *      Author: Clive Webster
 *
 *  Define structures common to all cameras
 *  and define the programming interface
 */

#ifndef _CAMERA_COMMON_H_
#define _CAMERA_COMMON_H_

#include "../libdefs.h"
#include "../uart.h"
#include "../color.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

// Define the structure of blob
typedef struct s_camera_blob {
	uint16_t	left;		// xMin
	uint16_t	right;		// xMax
	uint16_t	top;		// yMin
	uint16_t	bottom;		// yMax
	int16_t		xCenter;	// The x offset from the middle of the screen to the middle of the blob
	int16_t		yCenter;	// The y offset from the middle of the screen to the middle of the blob
	uint32_t	pixels;		// total size of the blob (width*height)
	uint8_t		bin;		// The color bin number
} CAMERA_BLOB;
#define MAKE_CAMERA_BLOB() { 0,0,0,0,0,0,0,0 }

// Define the structure of a colour bin - ie a min/max colour pair
typedef struct s_camera_bin {
	COLOR min;
	COLOR max;
	boolean dirty:1;	// TRUE if needs sending to camera
	boolean active:1;   // TRUE if the bin is in use
} CAMERA_BIN;

// Used by cameraGetBlobs to find blobs that match ANY camera bin color
#define CAMERA_ALL_BINS 255

// Fwd def
struct c_camera;

// Define the data for a generic camera
typedef struct s_camera {
	const struct /*PROGMEM*/ c_camera*	clazz;
	UART*		uart;			// The UART to connect to the camera
	CAMERA_BIN* bins;			// The colour bins
	CAMERA_BLOB* blobs;			// The blobs
	uint32_t    minBlobSize;	// Ignore blobs that are smaller than this
	uint8_t		numBlobs;		// The number of blobs obtained
} CAMERA;
#define MAKE_CAMERA(class, uart) { class, &((uart)->_uart_), NULL, NULL, 4, 0 }



// Define the API for camera commands
typedef struct /*PROGMEM*/ c_camera{
	void     (*init)(CAMERA* camera);		  // Initialise the camera
	uint16_t  (*xResolution)(CAMERA* camera); // Get the horizontal resolution
	uint16_t  (*yResolution)(CAMERA* camera); // Get the vertical   resolution
	uint8_t   numColorBins;		  		  	  // The number of colour bins supported
	uint8_t	  (*getBlobs)(CAMERA* camera, uint8_t bin); // Read blobs
	boolean   (*getPixel)(CAMERA* camera,uint16_t x, uint16_t y, COLOR * color);
	char*     (*getVersion)(CAMERA* camera);	// Get camera firmware version
	boolean   (*setBin)(CAMERA* camera, uint8_t bin,
			const COLOR* min,const COLOR* max);	// Set a colour bin

} CAMERA_CLASS;

#define MAKE_CAMERA_CLASS(init, xres, yres, bins, getBlobs, getPixel,getVer,setBin) { \
	init, xres, yres, bins, getBlobs, getPixel,getVer,setBin }

// Declare the public methods that can be called on a given camera
#define cameraInit(camera) 					_cameraInit(&((camera)->_camera_))
#define cameraXresolution(camera) 			_cameraXresolution(&((camera)->_camera_))
#define cameraYresolution(camera) 			_cameraYresolution(&((camera)->_camera_))
#define cameraNumColorBins(camera)  		_cameraNumColorBins(&((camera)->_camera_))
#define cameraSetBin(camera,bin,min,max)  	_cameraSetBin(&((camera)->_camera_),bin,min,max)
#define cameraGetBlobs(camera,bin)  		_cameraGetBlobs(&((camera)->_camera_),bin)
#define cameraFetchBlob(camera,blobNo)		_cameraFetchBlob(&((camera)->_camera_),blobNo)
#define cameraSetMinBlobSize(camera,minSize)_cameraSetMinBlobSize(&((camera)->_camera_),minSize)
#define cameraGetPixel(camera,x,y,color)	_cameraGetPixel(&((camera)->_camera_),x,y,color)
#define cameraGetVersion(camera)			_cameraGetVersion(&((camera)->_camera_))

// Declare the private methods that can be called on an abstract camera
extern void      _cameraInit(CAMERA* camera);
extern uint16_t  _cameraXresolution(CAMERA* camera);
extern uint16_t  _cameraYresolution(CAMERA* camera);
extern uint8_t   _cameraNumColorBins(const CAMERA* camera);
extern uint8_t   _cameraMaxBlobs(const CAMERA* camera);
extern boolean   _cameraSetBin(CAMERA* camera,uint8_t bin,const COLOR*min, const COLOR*max);
extern uint8_t   _cameraGetBlobs(CAMERA* camera,uint8_t bin);
extern const CAMERA_BLOB* _cameraFetchBlob(const CAMERA* camera, uint8_t blobNo);
extern boolean   _cameraGetPixel(CAMERA* camera,uint16_t x, uint16_t y, COLOR * color);
extern char*  	_cameraGetVersion(CAMERA* camera);
static __inline__ void _cameraSetMinBlobSize(CAMERA* camera, uint32_t minSize){
	camera->minBlobSize = minSize;
}

#if defined(BUILDING_LIBRARY)
// functions in _camera_common.c only for use by camera drivers
extern void	_cameraInsertBlob(CAMERA* camera, const CAMERA_BLOB* blob);
extern boolean _cameraSetBin_RGB(CAMERA* camera, uint8_t binNo, const COLOR* min, const COLOR* max);
extern boolean _cameraSetBin_YUV(CAMERA* camera, uint8_t binNo, const COLOR* min, const COLOR* max);
#endif

#ifdef __cplusplus
}
// ===================== C++ code =======================
#include <string.h>

class Blob : public CAMERA_BLOB{
public:
	Blob(){
		left = right = top = bottom = 0;
		xCenter = yCenter = 0;
		pixels = 0;
		bin = 0;
	}

	// cast to a CAMERA_BLOB*
	operator CAMERA_BLOB *(){
		return this;
	}

};

class Camera {
public:
	Camera(CAMERA* cstr){
		m_cstr = cstr;
	}
	uint16_t getXresolution(void) const{
		return _cameraXresolution(m_cstr);
	}
	uint16_t getYresolution(void) const{
		return _cameraYresolution(m_cstr);
	}
	uint8_t getNumColorBins(void) const{
		return _cameraNumColorBins(m_cstr);
	}
	boolean setBin(uint8_t bin, Color& min, Color& max)const{
		return _cameraSetBin(m_cstr,bin,min, max);
	}
	uint8_t getBlobs(uint8_t bin) const{
		return _cameraGetBlobs(m_cstr,bin);
	}
	void fetchBlob(uint8_t blobNo, Blob& rtn) const{
		memcpy(rtn, _cameraFetchBlob(m_cstr, blobNo), sizeof(CAMERA_BLOB) );
	}

	boolean getPixel(uint16_t x, uint16_t y, Color& color) const{
		return _cameraGetPixel(m_cstr,x,y,color);
	}

	char* getVersion(void) const{
		return _cameraGetVersion(m_cstr);
	}

	void setMinBlobSize(uint32_t minSize)const{
		_cameraSetMinBlobSize(m_cstr,minSize);
	}


private:
	CAMERA* m_cstr;
};
#endif

#endif /* _CAMERA_COMMON_H_ */
