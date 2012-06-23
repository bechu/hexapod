/*
 * $Id: pwm.h,v 1.2 2010/06/15 00:48:59 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: pwm.h,v $
 * Revision 1.2  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.1  2009/10/26 18:49:24  clivewebster
 * *** empty log message ***
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
 * pwm.h
 *
 *  Created on: 22-Oct-2009
 *      Author: Clive Webster
 *
 * Provide general hardware PWM support to allow for things like dimming LEDs
 *
 * This can only be performed on the PWM output pins of the timers.
 *
 */

#ifndef PWM_H_
#define PWM_H_

#include "iopin.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif


// ------------------------------------------

// Set up PWM on the given pin with the given frequency and duty cycle
// returns FALSE if errors or TRUE if set up correctly in which case actualDeciHertz,if not null,
// has the actual deciHertz.
boolean pwmInitDeciHertz(const IOPin* pin, uint32_t deciHertz, PERCENTAGE duty, uint32_t* actualDeciHertz);
boolean pwmInitHertz(const IOPin* pin, uint32_t hertz, PERCENTAGE duty, uint32_t* actualHertz);

// Turn PWM off for the given pin
void pwmOff(const IOPin* pin);

// Set the duty cycle
void pwmSetDutyCycle(const IOPin* pin, PERCENTAGE duty);

// Get the current duty cycle
PERCENTAGE pwmGetDutyCycle(const IOPin* pin);

#ifdef __cplusplus
}
class Pwm{
public:
	Pwm(const IOPin * cstr){m_cstr = cstr;};
	void setPercent(PERCENTAGE duty) const {
		pwmSetDutyCycle(m_cstr,duty);
	}
	PERCENTAGE getPercent(void) const {
		return pwmGetDutyCycle(m_cstr);
	}

private:
	const IOPin* m_cstr;
};
#endif

#endif /* PWM_H_ */
