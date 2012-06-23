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
 *        File: _rtc_common.h
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 *
 *  The base class for all real time clocks
 */

#ifndef _RTC_COMMON_H_
#define _RTC_COMMON_H_

#include "../libdefs.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

typedef struct s_date_time{
	uint16_t year;			/* actual year ie 2011 */
	uint8_t	month;			/* 1 to 12 */
	uint8_t	date;			/* 1 to 31 */
	uint8_t	hours;			/* 0 to 23 */
	uint8_t	minutes;		/* 0 to 59 */
	uint8_t	seconds;		/* 0 to 59 */
} DATE_TIME;
#define MAKE_DATE_TIME( y,m,d, h,min,s){ \
	y,m,d, h,min,s \
}

// Forward definition
struct /*PROGMEM*/ c_rtc_driver;

typedef struct s_rtc{
	const struct c_rtc_driver* clazz;
	DATE_TIME	dateTime;
} _RTC_;

#define MAKE_RTC(c) {c}

/*----------------------------------------------------------------------------------------
* Define the api for all rtc drivers ie they can: be initialised, read,
* or written
-----------------------------------------------------------------------------------------*/
typedef struct /*PROGMEM*/ c_rtc_driver{
	void 		(*init)(_RTC_*);
	DATE_TIME* 	(*read)(_RTC_*);
	boolean 	(*set)(_RTC_* _rtc_, const DATE_TIME* dt );
} RTC_DRIVER_CLASS;

#define MAKE_RTC_DRIVER_CLASS(init,read,write)  {init,read,write}

extern void _rtcInit(_RTC_* _rtc_);
extern const DATE_TIME* _rtcRead(_RTC_* _rtc_) __attribute__ ((warn_unused_result));
extern boolean _rtcSet(_RTC_* _rtc_, uint16_t y,uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t sec );

#define rtcInit(p)  _rtcInit(&(p)->_rtc_)
#define rtcRead(p)  _rtcRead(&(p)->_rtc_)
#define rtcSet(p, y,m,d, h,min,s) _rtcSet(&(p)->_rtc_, y,m,d, h,min,s)

// Dump the value of a sensor to the rprintf destination
void _rtcDumpTo(FILE* f,const _RTC_* device);
#define rtcDumpTo(f,device) _rtcDumpTo(f,	  &device._rtc_);
#define rtcDump(device) 	_rtcDumpTo(stdout,&device._rtc_);


#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class RTC{
public:
	RTC(_RTC_* cstr){
		m_cstr = cstr;
	};

	const DATE_TIME* read(void) {
		return _rtcRead(m_cstr);
	}

	boolean set(uint16_t y,uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s ){
		return _rtcSet(m_cstr, y,m,d, h,min,s);
	}

	void dumpTo(FILE* str=stdout) const{
		_rtcDumpTo(str,m_cstr);
	}

	void dump(void) const{
		dumpTo(stdout);
	}

private:
	_RTC_* m_cstr;

};
#endif


#endif /* _RTC_COMMON_H_ */
