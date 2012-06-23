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
 *        File: Matrix3Dtrans.c
 *  Created on: 24 Sep 2011
 *      Author: Clive
 */

#include "Matrix3D.h"

// dst = src x matrix
void matrix3d_Transform(VECTOR3D* dst, const VECTOR3D* src, const MATRIX3D* matrix ){
	double x,y,z;
	x = matrix->m00 * src->x + matrix->m01 * src->y + matrix->m02 * src->z;
	y = matrix->m10 * src->x + matrix->m11 * src->y + matrix->m12 * src->z;
	z = matrix->m20 * src->x + matrix->m21 * src->y + matrix->m22 * src->z;
	vector3d_Set(dst,x,y,z);
}
