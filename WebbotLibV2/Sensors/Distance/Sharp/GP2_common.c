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
 *        File: GP2_common.c
 *  Created on: 8 Feb 2011
 *      Author: Clive Webster
 */
#include "GP2.h"
#include "../../../a2d.h"
#include "../../../core.h"


DISTANCE_TYPE _gp2_read(ADC_CHANNEL adc, const SHARP* table, uint8_t entries){
	int16_t adc8 = (int16_t)a2dConvert8bit(adc);

	int16_t minAdc =0;
	int16_t maxAdc;
	while(maxAdc=(int16_t)(pgm_read_byte(&table->end_adc)),  adc8 > maxAdc ){
		minAdc = maxAdc+1;
		table++;
	}

	DISTANCE_TYPE minDist = pgm_read_word(&table->minDist);
	DISTANCE_TYPE maxDist = pgm_read_word(&table->maxDist);

	uint16_t result = interpolate(adc8,  minAdc,maxAdc,  (int16_t)minDist, (int16_t)maxDist);
	return (DISTANCE_TYPE)result;
}

