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
 *        File: Matrix2D.c
 *  Created on: 17 Jul 2011
 *      Author: Clive Webster
 */

#include "Matrix2D.h"
#include <math.h>
#include <string.h>

// Set a matrix to the given values
void matrix2d_Set(MATRIX2D* matrix,
				  double m00,double m01,
				  double m10,double m11){
	matrix->m00 = m00;
	matrix->m01 = m01;
	matrix->m10 = m10;
	matrix->m11 = m11;
}

// Copy one matrix to another
void matrix2d_Copy(MATRIX2D* dst, const MATRIX2D* src){
	if(src!=dst){
		memcpy(dst,src,sizeof(MATRIX2D));
	}
}

// Calculate the determinant of the matrix
double matrix2d_Determinant(const MATRIX2D* matrix){
	double total;

   total =  matrix->m00*matrix->m11 - matrix->m01*matrix->m10;
   return total;
}

// Set the matrix to a rotation
void matrix2d_SetRotate(MATRIX2D* matrix, double radians){
	double	sinAngle, cosAngle;

	sinAngle = sin(radians);
	cosAngle = cos(radians);

	matrix->m00 = cosAngle;
	matrix->m01 = -sinAngle;

	matrix->m10 = sinAngle;
	matrix->m11 = cosAngle;
}

// Dst = Dst * Src
void matrix2d_Multiply(MATRIX2D* dst, const MATRIX2D* src){
	MATRIX2D result;

		result.m00 = dst->m00*src->m00 + dst->m01*src->m10;
		result.m01 = dst->m00*src->m01 + dst->m01*src->m11;

		result.m10 = dst->m10*src->m00 + dst->m11*src->m10;
		result.m11 = dst->m10*src->m01 + dst->m11*src->m11;

		matrix2d_Copy(dst,&result);
}


// dst = src x matrix
void matrix2d_Transform(VECTOR2D* dst, const VECTOR2D* src, const MATRIX2D* matrix ){
	double x,y;
	x = matrix->m00 * src->x + matrix->m01 * src->y;
	y = matrix->m10 * src->x + matrix->m11 * src->y;
	vector2d_Set(dst,x,y);
}

// Set the matrix to a scale factor
void matrix2d_SetScale(MATRIX2D* matrix, double scale){
	matrix2d_Set(matrix, scale,0, 0,scale);
}
