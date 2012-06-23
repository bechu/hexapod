/*
 * $Id: HMC6352.h,v 1.4 2010/09/30 16:50:17 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: HMC6352.h,v $
 * Revision 1.4  2010/09/30 16:50:17  clivewebster
 * Refactored for new hw or sw i2c bus
 *
 * Revision 1.3  2010/06/14 18:53:23  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2010/02/04 19:57:12  clivewebster
 * Add functions to change the refresh rate
 *
 * Revision 1.1  2010/01/30 21:21:18  clivewebster
 * Added in Version 1.14
 *
 * ===========
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
 *
 * HMC6352.h
 *
 *  Created on: 29-Jan-2010
 *      Author: Clive Webster
 */

#ifndef HMC6352_H_
#define HMC6352_H_

#include "../_compass_common.h"
#include "../../../i2cBus.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

extern const COMPASS_CLASS c_HMC6352;

typedef struct s_HMC6352{
	COMPASS 	 compass;
	I2C_DEVICE	  i2cInfo;
}HMC6352;

#define MAKE_HMC6352() { \
	MAKE_COMPASS_SENSOR(c_HMC6352), \
	MAKE_I2C_DEVICE(0x42) \
	}

#define MAKE_HMC6352_At(i2cAddress) { \
	MAKE_COMPASS_SENSOR(c_HMC6352), \
	MAKE_I2C_DEVICE(i2cAddress) \
	}


// Set the default refresh rates
extern void HMC6352_1Hz(HMC6352* compass);
extern void HMC6352_5Hz(HMC6352* compass);
extern void HMC6352_10Hz(HMC6352* compass);
extern void HMC6352_20Hz(HMC6352* compass);

#ifdef __cplusplus
}
class Hmc6352 : public Compass{
public:
	Hmc6352(HMC6352* c) : Compass(&c->compass){
		m_hmc = c;
	}

	void refresh1Hz(void) const{
		HMC6352_1Hz(m_hmc);
	}

	void refresh5Hz(void) const{
		HMC6352_5Hz(m_hmc);
	}

	void refresh10Hz(void) const{
		HMC6352_10Hz(m_hmc);
	}

	void refresh20Hz(void) const{
		HMC6352_20Hz(m_hmc);
	}

private:
	HMC6352* m_hmc;

};
#endif

#endif /* HMC6352_H_ */
