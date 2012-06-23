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
 *        File: Vector3Dangle.c
 *  Created on: 24 Sep 2011
 *      Author: Clive
 */

#include "Vector3D.h"
#include <math.h>

double vector3d_AngleRadians(const VECTOR3D* v1, const VECTOR3D* v2){
	double vDot = vector3d_DotProduct(v1,v2) / ( vector3d_Length(v1) * vector3d_Length(v2) );
	if( vDot < -1.0){
		vDot = -1.0;
	}else if( vDot >  1.0){
		vDot =  1.0;
	}
	return acos( vDot );
}
