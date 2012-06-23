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
 *        File: Vector3Dnorm.c
 *  Created on: 24 Sep 2011
 *      Author: Clive
 */

#include "Vector3D.h"

// Change the vector in 'src' into a unit vector at 'dst' - ie with length = 1
// src and dst can be the same vector
void vector3d_Normalise(VECTOR3D* dst,const VECTOR3D* src){
	vector3d_Copy(dst,src);
	if(!dst->normalised){
		double dist = vector3d_Length(dst);
		if(dist==0.0){
			dst->x = 0.0;
			dst->y = 0.0;
			dst->z = 0.0;
		}else{
			dst->x /= dist;
			dst->y /= dist;
			dst->z /= dist;
		}
		dst->normalised = TRUE;
	}
}
