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
 *        File: Matrix3Ddet.c
 *  Created on: 24 Sep 2011
 *      Author: Clive
 */
#include "Matrix3D.h"

// Calculate the determinant of the matrix
double matrix3d_Determinant(const MATRIX3D* matrix){
	double total;

   total =  matrix->m00*(matrix->m11*matrix->m22 - matrix->m12*matrix->m21)
		  + matrix->m01*(matrix->m12*matrix->m20 - matrix->m10*matrix->m22)
		  + matrix->m02*(matrix->m10*matrix->m21 - matrix->m11*matrix->m20);
   return total;
}
