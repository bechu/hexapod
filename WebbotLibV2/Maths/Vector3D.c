/*
 * $Id: Vector3D.c,v 1.1 2010/07/02 01:57:39 clivewebster Exp $
 * Revision History
 * ================
 * $Log: Vector3D.c,v $
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
 *        File: Vector3D.c
 *  Created on: 23 Jun 2010
 *      Author: Clive Webster
 */

#include "Vector3D.h"
#include <math.h>
#include <string.h>


void vector3d_Copy(VECTOR3D* dst,const VECTOR3D* src){
	if(src!=dst){
		memcpy(dst,src,sizeof(VECTOR3D));
	}
}



void vector3d_Scale(VECTOR3D* v, double scale){
	v->x *= scale;
	v->y *= scale;
	v->z *= scale;
	v->normalised = (v->normalised && scale==1.0) ? TRUE : FALSE;
}

// Find the dot product of two vectors
double vector3d_DotProduct(const VECTOR3D* v1, const VECTOR3D* v2){
	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);

}



void vector3d_SetX(VECTOR3D* vector, double x){
	if(x != vector->x){
		vector->x = x;
		vector->normalised=FALSE;
	}
}

void vector3d_SetY(VECTOR3D* vector, double y){
	if(y != vector->y){
		vector->y = y;
		vector->normalised=FALSE;
	}
}

void vector3d_SetZ(VECTOR3D* vector, double z){
	if(z != vector->z){
		vector->z = z;
		vector->normalised=FALSE;
	}
}

void vector3d_Set(VECTOR3D* vector, double x, double y, double z){
	vector->x = x;
	vector->y = y;
	vector->z = z;
	vector->normalised=FALSE;
}
void vector3d_CrossProduct(VECTOR3D*result, const VECTOR3D* v1, const VECTOR3D* v2){

	VECTOR3D rtn;

	rtn.x = v1->y * v2->z - v1->z * v2->y;
	rtn.y = v2->x * v1->z - v2->z * v1->x;
	rtn.z = v1->x * v2->y - v1->y * v2->x;

	vector3d_Copy(result,&rtn);
}




