/*
 * $Id: Matrix3D.c,v 1.1 2010/07/02 01:57:39 clivewebster Exp $
 * Revision History
 * ================
 * $Log: Matrix3D.c,v $
 * Revision 1.1  2010/07/02 01:57:39  clivewebster
 * Added
 *
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
 *        File: Matrix3D.c
 *  Created on: 23 Jun 2010
 *      Author: Clive Webster
 */
#include "Matrix3D.h"
#include <math.h>
#include <string.h>

// Set a matrix to the given values
void matrix3d_Set(MATRIX3D* matrix,
				  double m00,double m01,double m02,
				  double m10,double m11,double m12,
				  double m20,double m21,double m22){
	matrix->m00 = m00;
	matrix->m01 = m01;
	matrix->m02 = m02;
	matrix->m10 = m10;
	matrix->m11 = m11;
	matrix->m12 = m12;
	matrix->m20 = m20;
	matrix->m21 = m21;
	matrix->m22 = m22;
}

void matrix3d_Zero(MATRIX3D* matrix){
	matrix3d_SetScale(matrix, 0.0);
}

void matrix3d_Unity(MATRIX3D* matrix){
	matrix3d_SetScale(matrix, 1.0);
}

// Copy one matrix to another
void matrix3d_Copy(MATRIX3D* dst, const MATRIX3D* src){
	if(src!=dst){
		memcpy(dst,src,sizeof(MATRIX3D));
	}
}




