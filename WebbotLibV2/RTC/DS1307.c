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
 *        File: DS1307.c
 *  Created on: 24 Jul 2011
 *      Author: Clive Webster
 */
#include "DS1307.h"

#define REG_SEC 0
#define REG_MIN 1
#define REG_HOUR 2
#define REG_DOFW 3
#define REG_DATE 4
#define REG_MONTH 5
#define REG_YEAR 6
#define REG_CTRL 7


// Convert normal decimal numbers to binary coded decimal
static uint8_t toBCD(uint8_t val)
{
  return ( ( (val/10)<<4) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
static uint8_t fromBCD(uint8_t val)
{
  return ( ( (val>>4) *10) + (val & 15) );
}


static void init(_RTC_* _rtc_){
	DS1307_RTC* rtc = (DS1307_RTC*)_rtc_;

	// Make sure that the clock is enabled
	uint8_t reg;
	if(i2cMasterReadRegisters(&rtc->i2cInfo,REG_SEC,1,&reg)){
		if(reg & 0x80){
			i2cMasterWriteRegister(&rtc->i2cInfo,REG_SEC,reg & 0x7f);
		}
	}

	// Turn off the pulse output
	i2cMasterWriteRegister(&rtc->i2cInfo,REG_CTRL,0);

	// Make sure the clock is in 24 hour setting
	if(i2cMasterReadRegisters(&rtc->i2cInfo,REG_HOUR,1,&reg)){
		// We want a 24 hour clock
		if(reg & 0x40){
			// Its in 12 hour mode
			uint8_t h = fromBCD(reg & 0x1f);	// Current hour from 1 to 12
			// Special handling for hour=12
			if(h==12){
				if( (reg & 0x20)==0){
					// Its 12am so make 0
					h = 0;
				}
			}else{
				// if 1pm to 11pm then just add 12
				if(reg & 0x20){
					h += 12;
				}
			}
			i2cMasterWriteRegister(&rtc->i2cInfo,REG_HOUR,toBCD(h));
		}
	}
}

static DATE_TIME* read(_RTC_* _rtc_){
	DS1307_RTC* rtc = (DS1307_RTC*)_rtc_;
	DATE_TIME* rtn = null;

	uint8_t rtc_response[7];
	if(i2cMasterReadRegisters(&rtc->i2cInfo, 0, sizeof(rtc_response), rtc_response)){
		// We have successfully read the data into 'rtc_response'
		rtn = &_rtc_->dateTime;
		rtn->seconds = fromBCD(rtc_response[REG_SEC] & 0x7f);
		rtn->minutes = fromBCD(rtc_response[REG_MIN] & 0x7f);

		// 24 hour mode
		rtn->hours   = fromBCD(rtc_response[REG_HOUR] & 0x3f); // 00 to 23

		// Get date
		rtn->date = fromBCD(rtc_response[REG_DATE] & 0x3f);
		rtn->month = fromBCD(rtc_response[REG_MONTH] & 0x1f);
		rtn->year = 2000 + fromBCD(rtc_response[REG_YEAR]);
	}
	return rtn;
}

static boolean write(_RTC_* _rtc_, const DATE_TIME* dt ){
	DS1307_RTC* rtc = (DS1307_RTC*)_rtc_;

	uint8_t rtc_response[7];

	rtc_response[REG_SEC] = toBCD(dt->seconds);
	rtc_response[REG_MIN] = toBCD(dt->minutes);
	rtc_response[REG_HOUR] = toBCD(dt->hours);

	// Just make day of week valid
	rtc_response[REG_DOFW] = 1;

	rtc_response[REG_DATE] = toBCD(dt->date);
	rtc_response[REG_MONTH] = toBCD(dt->month);
	rtc_response[REG_YEAR] = toBCD(dt->year % 100);

	return i2cMasterWriteRegisters(&rtc->i2cInfo,REG_SEC,sizeof(rtc_response), rtc_response);
}

const RTC_DRIVER_CLASS PROGMEM c_ds1307_rtc = MAKE_RTC_DRIVER_CLASS(&init,&read,&write);
