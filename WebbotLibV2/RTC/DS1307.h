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
 *        File: DS1307.h
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "_rtc_common.h"
#include "../i2cBus.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

extern const RTC_DRIVER_CLASS c_ds1307_rtc;

typedef struct s_ds1307_rtc{
	_RTC_	_rtc_;			// include common stuff (must be first entry)
	I2C_DEVICE	  i2cInfo;
}DS1307_RTC;

#define MAKE_DS1307() { \
	MAKE_RTC(&c_ds1307_rtc), \
	MAKE_I2C_DEVICE(0xD0)	 \
}

#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class DS1307 : public RTC{
public:
	DS1307(DS1307_RTC* c) : RTC(&c->_rtc_){
	}

};
#endif


#endif /* DS1307_H_ */
