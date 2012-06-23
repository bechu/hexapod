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
 *        File: DS3234.h
 *  Created on: 25 Jul 2011
 *      Author: Clive Webster
 */

#ifndef DS3234_H_
#define DS3234_H_

#include "_rtc_common.h"
#include "../_spi_common.h"


#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

extern const RTC_DRIVER_CLASS c_ds3234_rtc;

typedef struct s_ds3234_rtc{
	_RTC_	_rtc_;						// include common stuff (must be first entry)
	SPI_ABSTRACT_DEVICE	_device_;		// Its an SPI device
}DS3234_RTC;

#define MAKE_DS3234(select) { \
	MAKE_RTC(&c_ds3234_rtc), \
	MAKE_SPI_DEVICE(SPI_MODE_1,SPI_DATA_ORDER_MSB,select,0x00)	 \
}


#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class DS3234 : public RTC{
public:
	DS3234(DS3234_RTC* c) : RTC(&c->_rtc_){
	}

};
#endif


#endif /* DS3234_H_ */
