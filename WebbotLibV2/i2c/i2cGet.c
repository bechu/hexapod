/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
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
 *        File: i2cGet.c
 *  Created on: 31 Dec 2010
 *      Author: Clive Webster
 */
#include "../i2cBus.h"
#include "../errors.h"

uint8_t i2cGet(const I2C_ABSTRACT_BUS* i2c, boolean isLastByte){
	uint8_t rtn = 0;
	if(i2c){
		if(i2c->initialised){
			const I2C_CLASS* class = i2c->sclass;
			uint8_t (*fn)(const I2C_ABSTRACT_BUS*, boolean) =
					(uint8_t (*)(const I2C_ABSTRACT_BUS*,boolean))pgm_read_word(&class->get);
			rtn = fn(i2c, isLastByte); 	// call the get method
		}else{
			setError(I2C_BUS_NOT_INITIALIZED);
		}
	}
	return rtn;
}

