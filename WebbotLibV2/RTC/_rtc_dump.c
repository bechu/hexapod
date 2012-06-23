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
 *        File: _rtc_dump.c
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 */
#include "_rtc_common.h"
#include "../core.h"

void _rtcDumpTo(FILE* f,const _RTC_* device){
	const DATE_TIME* t = &device->dateTime;
	PRINTF(stdout,"%02u/%02u/%04u %02u:%02u:%02u\r\n", t->date, t->month, t->year, t->hours, t->minutes, t->seconds);
}
