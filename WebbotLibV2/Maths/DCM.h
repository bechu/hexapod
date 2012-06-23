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
 *        File: DCM.h
 *  Created on: 28 Sep 2011
 *      Author: Clive Webster
 *
 *  A generic Directional Cosine Matrix class
 */

#ifndef DCM_H_
#define DCM_H_

#include "Matrix3D.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif



#ifdef __cplusplus
}
/* ==================== C++ Code ==============================================*/
class DCM  {

public:
	// Construct a new matrix
	DCM(void);

	// Reset everything
	void reset(void);

	// Update matrix with gyro info
	void gyroDegrees(const Vector3D& gyroDegPerSec);
	void gyroRadians(const Vector3D& gyroRadPerSec);

	// Correct any drift - accelVector in whole G ie 1.000 = 1G, and heading in radians
	void driftCorrection(const Vector3D& accelVector, double magHeadingRadians);

	// Get Attitude in radians
	double getPitchRadians(void) const;
	double getRollRadians(void) const;
	double getYawRadians(void) const;

	// Get Attitude in degrees
	double getPitchDegrees(void) const;
	double getRollDegrees(void) const;
	double getYawDegrees(void) const;

public:
	double m_Kp_ROLLPITCH,m_Ki_ROLLPITCH;
	double m_Kp_YAW,m_Ki_YAW;

private:
	Matrix3D m_mat;
	Vector3D m_omega_P;
	Vector3D m_omega_I;
	TICK_COUNT m_lastTime;
	boolean m_started;
};


#endif


#endif /* DCM_H_ */
