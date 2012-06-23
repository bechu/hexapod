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
 *        File: _rtc_set.c
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 */

#include "_rtc_common.h"


boolean _rtcSet(_RTC_* _rtc_, uint16_t y,uint8_t m,uint8_t d, uint8_t h,uint8_t min,uint8_t s){
	const RTC_DRIVER_CLASS* clazz = _rtc_->clazz;
	boolean rtn = FALSE;

	// Put the fields into a sensible range
	m = CLAMP(m, 1, 12);
	d = CLAMP(d, 1, 31);
	h = CLAMP(h, 0, 23);
	min = CLAMP(min, 0, 59);
	s = CLAMP(s, 0, 59);

	DATE_TIME dt = MAKE_DATE_TIME( y,m,d, h,min,s);


	boolean (*fn)(_RTC_*, const DATE_TIME*) = (boolean (*)(_RTC_*, const DATE_TIME*))pgm_read_word(&clazz->set);
	if(fn){
		rtn = fn(_rtc_, &dt);
	}
	return rtn;

}
