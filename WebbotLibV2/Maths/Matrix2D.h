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
 *        File: Matrix2D.h
 *  Created on: 17 Jul 2011
 *      Author: Clive Webster
 */

#ifndef MATRIX2D_H_
#define MATRIX2D_H_

#include "../libdefs.h"
#include "Vector2D.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

typedef struct s_matrix2d {
	double	m00,m01;	// top row
	double	m10,m11;	// bottom row
} MATRIX2D;

// Construct matrix with given values
#define MAKE_MATRIX2D(m00,m01, m10,m11) {m00,m01, m10,m11}
// Construct matrix with all values as zero
#define MAKE_ZERO_MATRIX2D() MAKE_MATRIX2D{0,0, 0,0}
// Construct identity matrix
#define MAKE_IDENTITY_MATRIX2D() MAKE_MATRIX2D{1,0, 0,1}

// Set a matrix to the given values
void matrix2d_Set(MATRIX2D* matrix,
				  double m00,double m01,
				  double m10,double m11);

// Copy one matrix to another
void matrix2d_Copy(MATRIX2D* dst, const MATRIX2D* src);

// Calculate the determinant of the matrix
double matrix2d_Determinant(const MATRIX2D* matrix);

// Set the matrix to a rotation
void matrix2d_SetRotate(MATRIX2D* matrix, double radians);

// Set the matrix to a scale factor
void matrix2d_SetScale(MATRIX2D* matrix, double scale);

// Dst = Dst * Src
void matrix2d_Multiply(MATRIX2D* dst, const MATRIX2D* src);

// dst = src x matrix
void matrix2d_Transform(VECTOR2D* dst, const VECTOR2D* src, const MATRIX2D* matrix);

#ifdef __cplusplus
}
#endif

#endif /* MATRIX2D_H_ */
