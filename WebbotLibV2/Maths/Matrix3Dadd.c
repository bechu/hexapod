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
 *        File: Matrix3Dadd.c
 *  Created on: 24 Sep 2011
 *      Author: Clive
 */
#include "Matrix3D.h"

// Dst = Dst + Src
void matrix3d_Add(MATRIX3D* dst, const MATRIX3D* src){
	dst->m00 += src->m00;
	dst->m01 += src->m01;
	dst->m02 += src->m02;

	dst->m10 += src->m10;
	dst->m11 += src->m11;
	dst->m12 += src->m12;

	dst->m20 += src->m20;
	dst->m21 += src->m21;
	dst->m22 += src->m22;
}

