/*
 * $Id: ITG3200.h,v 1.2 2011/01/25 23:49:38 clivewebster Exp $
 * Revision History
 * ================
 * $Log: ITG3200.h,v $
 * Revision 1.2  2011/01/25 23:49:38  clivewebster
 * Auto calibrate the zer position on start up
 *
 * Revision 1.1  2011/01/05 00:51:41  clivewebster
 * Added ITG3200 3 axis I2C gyro sensor
 *
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
 *        File: ITG3200.h
 *  Created on: 3 Jan 2011
 *      Author: Clive Webster
 */

#ifndef ITG3200_H_
#define ITG3200_H_
#include "../_gyro_common.h"
#include "../../Temperature/_temperature_common.h"
#include "../../../i2cBus.h"

#ifdef __cplusplus
/* ===================== C Code =============================================== */
extern "C" {
#endif

// The class used to read a raw value
extern const GYRO_CLASS c_ITG3200;

typedef struct s_ITG3200{
	GYRO 		 gyro;
	I2C_DEVICE	 i2cInfo;
	TEMPERATURE_TYPE celsius;
	int16_t		x_zero,y_zero,z_zero;
} ITG3200;

// Create the sensor using X, Y pins
#define MAKE_ITG3200_At(addr) { \
	MAKE_GYRO_SENSOR(c_ITG3200), \
	MAKE_I2C_DEVICE(addr),0, \
	0,0,0 \
	}

// Use the default I2C address
#define MAKE_ITG3200() MAKE_ITG3200_At(0xD0)

#ifdef __cplusplus
}
#include "../../Temperature/_temperature_common.h"
class Itg3200 : public Gyro{
public:
	Itg3200(ITG3200* cstr) : Gyro(&cstr->gyro){
		m_cstr = cstr;
	}
	TEMPERATURE_TYPE getCelsius(void) const{
		return m_cstr->celsius;
	}
	TEMPERATURE_TYPE getFahrenheit(void) const{
		return Temperature::toFahrenheit(getCelsius());
	}
private:
	ITG3200* m_cstr;
};

#endif

#endif /* ITG3200_H_ */
