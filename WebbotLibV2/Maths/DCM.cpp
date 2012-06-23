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
 *        File: DCM.cpp
 *  Created on: 28 Sep 2011
 *      Author: Clive
 */
#include "DCM.h"
#include "../clock.h"

#include <math.h>

#define ToRad(x) (x * (M_PI/180.0))
#define ToDeg(x) (x * (180.0/M_PI))

//#define DEBUG cout




// Initialise to the unit vector
DCM::DCM(void){
	reset();

	// Original code had gravity as a value of 248 but now it is just 1.0
	m_Kp_ROLLPITCH  = 0.02 * 248.0;
	m_Ki_ROLLPITCH = 0.00002 * 248.0;

	m_Kp_YAW =  1.2;
	m_Ki_YAW = 0.00002;
}

void DCM::reset(void){
	m_mat.unit();
	m_omega_I.set(0,0,0);
	m_omega_P.set(0,0,0);
	m_lastTime = 0;
	m_started = FALSE;
}

// Update matrix with gyro info
void DCM::gyroDegrees(const Vector3D& gyroDegPerSec){
	Vector3D rad = gyroDegPerSec;
	rad *= ToRad(1);			// scale to radians
	gyroRadians(rad);
}

// Update matrix with gyro info
void DCM::gyroRadians(const Vector3D& gyroRadPerSec){

	// Calc the duration in whole seconds since last called
	TICK_COUNT now = clockGetus();
	TICK_COUNT elapsed;
	if(!m_started){
		// first time in
		m_started = TRUE;
		elapsed = 0;
	}else{
		elapsed = now - m_lastTime;
	}
	m_lastTime = now;
	double dt = elapsed;			// in uS
	dt /= 1000000.0;			// in whole seconds

	// Omega = (gyro + proportional + integration) * timediff
	Vector3D omegaVector(gyroRadPerSec);
	omegaVector += m_omega_P;
	omegaVector += m_omega_I;
	omegaVector *= dt;


	Matrix3D delta(   		   0.0, -omegaVector.z,  omegaVector.y,
					 omegaVector.z,            0.0, -omegaVector.x,
					-omegaVector.y,  omegaVector.x,            0.0);



	// DCM_Matrix += DCM_Matrix * delta;
	Matrix3D temp(m_mat);
	temp *= delta;
	m_mat += temp;


#ifdef DEBUG
	DEBUG.print_P(PSTR("Omega:")); omegaVector.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("Delta:"));delta.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("temp:"));temp.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("New DCM:")); m_mat.dump(DEBUG);DEBUG.println();
#endif


	// ---------- normalise ----------

	Vector3D dcm0,dcm1,dcm2,temp0,temp1,temp2;

	dcm0.set(m_mat.m00,m_mat.m01,m_mat.m02);	// Row 0 of DCM
	dcm1.set(m_mat.m10,m_mat.m11,m_mat.m12);	// Row 1 of DCM
	dcm2.set(m_mat.m20,m_mat.m21,m_mat.m22);	// Row 2 of DCM

	// Equation 19
	double error = -0.5 * dcm0.dot(dcm1);
	temp0 = dcm1; temp0*=error; temp0+=dcm0;
	temp1 = dcm0; temp1*=error; temp1+=dcm1;
	// Equation 20 - Use cross product to find 3rd value
	temp2 = temp0.cross(temp1);


	// Apply Equation 21 to each axis
  	double renorm;
  	renorm = 0.5 * (3.0 - temp0.dot(temp0)); temp0 *= renorm;
  	renorm = 0.5 * (3.0 - temp1.dot(temp1)); temp1 *= renorm;
  	renorm = 0.5 * (3.0 - temp2.dot(temp2)); temp2 *= renorm;

	// Set DCM to the temp vectors
	m_mat.set( temp0.x,temp0.y,temp0.z, temp1.x,temp1.y,temp1.z, temp2.x,temp2.y,temp2.z);

#ifdef DEBUG
	DEBUG.print_P(PSTR("Normalised DCM:")); m_mat.dump(DEBUG);DEBUG.println();
#endif

	// Proportional starts from 0 whereas integral keeps adding
  	m_omega_P.set(0,0,0);
}

void DCM::driftCorrection(const Vector3D& accelVector, double magHeadingRadians){


	//*****Roll and Pitch***************

  	// Calculate the magnitude of the accelerometer vector (in whole G)
  	double Accel_magnitude = accelVector.length();


  	// Dynamic weighting of accelerometer info (reliability filter)
  	// Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
	double weight = 1.0 - 2.0*ABS(1.0 - Accel_magnitude);
  	double Accel_weight = CLAMP(weight,0.0,1.0);  //



	Vector3D dcm2(m_mat.m20,m_mat.m21,m_mat.m22);

	Vector3D errorRollPitch =  accelVector.cross(dcm2);
	m_omega_P += errorRollPitch * (m_Kp_ROLLPITCH*Accel_weight);							//adjust the ground of reference
	m_omega_I += errorRollPitch * (m_Ki_ROLLPITCH*Accel_weight);

  	//*****YAW***************

  	// We make the gyro YAW drift correction based on compass magnetic heading
	double mag_heading_x = cos(magHeadingRadians);
	double mag_heading_y = sin(magHeadingRadians);
	double errorCourse=(m_mat.m00*mag_heading_y) - (m_mat.m10*mag_heading_x);  			//Calculating YAW error
	Vector3D errorYaw = dcm2 * errorCourse;												//Applies the yaw correction to the XYZ rotation of the aircraft, depending the position.
	m_omega_P += errorYaw * m_Kp_YAW;
	m_omega_I += errorYaw * m_Ki_YAW;

#ifdef DEBUG
	DEBUG.print_P(PSTR("AccelMag="));DEBUG.print(Accel_magnitude);DEBUG.println();
	DEBUG.print_P(PSTR("weight="));DEBUG.print(weight);DEBUG.println();
	DEBUG.print_P(PSTR("AccelWeight="));DEBUG.print(Accel_weight);DEBUG.println();
	DEBUG.print_P(PSTR("DCM2="));dcm2.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("ErrRollPitch="));errorRollPitch.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("ErrYaw="));errorYaw.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("Omega_P="));m_omega_P.dump(DEBUG);DEBUG.println();
	DEBUG.print_P(PSTR("Omega_I="));m_omega_I.dump(DEBUG);DEBUG.println();
#endif

}

// Get Attitude in radians
double DCM::getPitchRadians(void) const{
	return -asin(m_mat.m20);
}

double DCM::getRollRadians(void) const{
	return atan2(m_mat.m21,m_mat.m22);
}
double DCM::getYawRadians(void) const{
	return atan2(m_mat.m10,m_mat.m00);
}

// Get Attitude in degrees
double DCM::getPitchDegrees(void) const{
	return ToDeg(getPitchRadians());
}
double DCM::getRollDegrees(void) const{
	return ToDeg(getRollRadians());
}
double DCM::getYawDegrees(void) const{
	return ToDeg(getYawRadians());
}

