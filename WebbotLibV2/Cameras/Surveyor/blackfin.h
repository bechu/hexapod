/*
 * $Id: blackfin.h,v 1.8 2010/07/15 14:36:57 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: blackfin.h,v $
 * Revision 1.8  2010/07/15 14:36:57  clivewebster
 * Allow Project Designer to set the initial resolution
 *
 * Revision 1.7  2010/06/14 18:41:37  clivewebster
 * Add copyright license info
 *
 * Revision 1.6  2010/02/04 19:46:57  clivewebster
 * Updated a comment
 *
 * Revision 1.5  2009/12/27 21:53:17  clivewebster
 * *** empty log message ***
 *
 * Revision 1.4  2009/12/13 19:21:10  clivewebster
 * Changed MAKE_BLACKFIN_CAMERA to something more intuitive
 *
 * Revision 1.3  2009/12/11 17:09:10  clivewebster
 * Fixed #include for Unix
 *
 * Revision 1.2  2009/11/02 18:34:07  clivewebster
 * Added revision log
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
 * blackfin.h
 *
 *  Created on: 11-Aug-2009
 *      Author: Clive Webster
 *
 *  Support for the Blackfin camera
 */

#ifndef BLACKFIN_H_
#define BLACKFIN_H_

//#define BLACKFIN_DEBUG

#include "../_camera_common.h"

//#include "../../_uart_common.h"
//#include "../../color.h"
//#include "../../libdefs.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif


// Class definition for a blackfin
extern const CAMERA_CLASS c_blackfin;

// Define the different camera resolution
typedef enum {
 	BLACKFIN_160_BY_120,
 	BLACKFIN_320_BY_240,
 	BLACKFIN_640_BY_480,
 	BLACKFIN_1280_BY_1024
} BLACKFIN_RESOLUTION;

// To override the default resolution then add the following to your code:-
// #define BLACKFIN_RESOLUTION_DEFAULT BLACKFIN_1280_BY_1024
#ifndef BLACKFIN_RESOLUTION_DEFAULT
#define BLACKFIN_RESOLUTION_DEFAULT BLACKFIN_160_BY_120
#endif


typedef struct s_blackfin {
	CAMERA _camera_;			// Include common camera data
	BLACKFIN_RESOLUTION res; 	// The current resolution of the camera
	COLOR*				mean;	// The last read mean value of the image
#ifdef BLACKFIN_DEBUG
	UART*    			debug;	// The UART used to send debug output to
#endif
} BLACKFIN_CAMERA;

// Routines that are private to the library - dont change these as it will mean rebuilding the library
#ifdef BUILDING_LIBRARY
//#define BLACKFIN_DEBUG
int __blackfin_get_args(const BLACKFIN_CAMERA* camera,int32_t * values, size_t numValues, boolean trace);
int _blackfin_putcmd(uint8_t data);
int __blackfinCommand(const BLACKFIN_CAMERA* camera, const char* response, const char* error, int32_t * values, size_t numValues);
extern size_t _blackfin_index;
void _blackfin_sendYUV(uint8_t c);

// Initialise the camera
//void _blackfinInit(BLACKFIN_CAMERA* camera);

boolean blackfinSendBins(BLACKFIN_CAMERA* camera);

// Detect the blobs for a given color bin
// Returns the number of blobs - which can be 0 (none) up to 16
//
uint8_t _blackfinDetectBlobs(CAMERA* camera, uint8_t bin);

// Get the pixel at a given location and write to the specified COLOR variable
boolean _blackfinGetPixel(CAMERA* camera,uint16_t x, uint16_t y, COLOR * color);


#endif




// Define the standard constructor
// The first parameter is the UART used to connect to the camera
// The second parameter is the UART used to write out debug info - or null if not required
#ifdef BLACKFIN_DEBUG
#define MAKE_BLACKFIN_CAMERA_RES(uart,res,debug)  { MAKE_CAMERA(&c_blackfin,uart) ,res,null,&((debug)->_uart_)}
#define MAKE_BLACKFIN_CAMERA(uart,debug)  MAKE_BLACKFIN_CAMERA_RES(uart,BLACKFIN_RESOLUTION_DEFAULT,debug)
#else
#define MAKE_BLACKFIN_CAMERA_RES(uart,res)  {MAKE_CAMERA(&c_blackfin,uart) ,res,null}
#define MAKE_BLACKFIN_CAMERA(uart)  MAKE_BLACKFIN_CAMERA_RES(uart,BLACKFIN_RESOLUTION_DEFAULT)
#endif

static __inline__ BLACKFIN_RESOLUTION blackfinGetResolution(const BLACKFIN_CAMERA* camera){
	return camera->res;
};


// Set the resolution of the camera
void blackfinSetResolution(BLACKFIN_CAMERA* camera, BLACKFIN_RESOLUTION res);


// Calculate the mean color
const COLOR * blackfinMeanColor(BLACKFIN_CAMERA* camera);

// Return the number of pixels in a rectangle that match a given color bin
uint32_t blackfinCountPixels(BLACKFIN_CAMERA* camera,uint8_t bin, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);


#ifdef BLACKFIN_DEBUG
Writer _blackfin_set_active(UART* uart);
#endif

#ifdef __cplusplus
}
#include <string.h>
class Blackfin : public Camera {
public:
	Blackfin(BLACKFIN_CAMERA* cstr) : Camera(&cstr->_camera_){
		m_blackfin = cstr;
	}

	BLACKFIN_RESOLUTION getResolution(void) const{
		return blackfinGetResolution(m_blackfin);
	}

	void setResolution(BLACKFIN_RESOLUTION res) const{
		 blackfinSetResolution(m_blackfin, res);
	}

	uint32_t countPixels(uint8_t bin, uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2) const{
		return blackfinCountPixels(m_blackfin,bin,x1,x2,y1,y2);
	}

	uint32_t countPixels(uint8_t bin) const{
		return countPixels(bin,0,getXresolution()-1,0,getYresolution()-1);
	}

	void getMeanColor(Color& rtn) const {
		const COLOR * mean = blackfinMeanColor(m_blackfin);
		memcpy(rtn, mean, sizeof(COLOR));
	}

private:
	BLACKFIN_CAMERA* m_blackfin;
};

#endif

#endif /* BLACKFIN_H_ */
