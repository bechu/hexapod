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
 *        File: _rtc_common.c
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 */

#include "_rtc_common.h"


void _rtcInit(_RTC_* _rtc_){
	const RTC_DRIVER_CLASS* clazz = _rtc_->clazz;

	void (*fn)(_RTC_*) = (void (*)(_RTC_*))pgm_read_word(&clazz->init);
	if(fn){
		fn(_rtc_);
	}

}

const DATE_TIME* _rtcRead(_RTC_* _rtc_){
	const RTC_DRIVER_CLASS* clazz = _rtc_->clazz;
	const DATE_TIME* rtn = null;

	const DATE_TIME* (*fn)(_RTC_*) = (const DATE_TIME* (*)(_RTC_*))pgm_read_word(&clazz->read);
	if(fn){
		rtn = fn(_rtc_);
	}
	return rtn;
}

