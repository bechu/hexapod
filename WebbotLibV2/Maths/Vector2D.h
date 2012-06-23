/*
 * $Id: Vector2D.h,v 1.1 2010/07/02 01:57:39 clivewebster Exp $
 * Revision History
 * ================
 * $Log: Vector2D.h,v $
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
 *        File: Vector2D.h
 *  Created on: 17 Jun 2010
 *      Author: Clive Webster
 *
 *  Perform 2 dimensional Vector algebra
 */

#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include "../libdefs.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

typedef struct s_vector2d {
	double	x;
	double	y;
	boolean	normalised;
} VECTOR2D;

#define MAKE_VECTOR2D(x,y) { x, y, FALSE }

static __inline__ double vector2d_GetX(const VECTOR2D* vector){
	return vector->x;
}

static __inline__ double vector2d_GetY(const VECTOR2D* vector){
	return vector->y;
}

void vector2d_SetX(VECTOR2D* vector, double x);
void vector2d_SetY(VECTOR2D* vector, double y);

static __inline__ void vector2d_Set(VECTOR2D* vector, double x, double y){
	vector2d_SetX(vector,x);
	vector2d_SetY(vector,y);
}

// Calculate the distance of the vector. ie sqrt(vector2d_LengthSquared(vector))
double vector2d_Length(const VECTOR2D* vector);

// Calculate the length squared of the vector
double vector2d_LengthSquared(const VECTOR2D* vector);

// Change the vector in 'src' into a unit vector at 'dst' - ie with length = 1
// src and dst can be the same vector
void vector2d_Normalise(VECTOR2D* dst,const VECTOR2D* src);

// Add the src vector to the dst vector
void vector2d_Add(VECTOR2D* dst,const VECTOR2D* src);

// Subtract the src vector from the dst vector
void vector2d_Subtract(VECTOR2D* dst,const VECTOR2D* src);

// Set the dst vector to the src vector
void vector2d_Copy(VECTOR2D* dst,const VECTOR2D* src);


// Scale the vector by a constant value
void vector2d_Scale(VECTOR2D* v,double scale);

// Find the dot product of two vectors
// returns |v1| * |v2| * cos(angle between v1,v2)
double vector2d_DotProduct(const VECTOR2D* v1, const VECTOR2D* v2);

// Returns the angle in radians between two vectors
// the return value is constrained to the range 0 ... PI

double vector2d_AngleRadians(const VECTOR2D* v1, const VECTOR2D* v2);

// round the vector to integer values
void vector2d_Round(VECTOR2D* dst,const VECTOR2D* src);

#ifdef __cplusplus
}
#include "../Stream/Stream.h"

class Vector2D : public VECTOR2D{
public:
	// Construct a new vector
	Vector2D(void){
		this->x = 0;
		this->y = 0;
		this->normalised = FALSE;
	}

	Vector2D( double x, double y){
		this->x = x;
		this->y = y;
		this->normalised = FALSE;
	}

	Vector2D( const Vector2D& rhs){
		vector2d_Copy(this,&rhs );
	}

	// Set this vector to the contents of another
	const Vector2D& operator = (const Vector2D& rhs){
		vector2d_Copy(this, &rhs );
		return *this;
	}

	// Test for equality
	boolean operator == (const Vector2D& dest) const{
		return equals(dest);
	}

	// Test for in-equality
	boolean operator != (const Vector2D& dest) const{
		return (equals(dest)) ? FALSE : TRUE;
	}

	// Add rhs to this
    Vector2D& operator +=( const Vector2D& rhs ){
    	vector2d_Add(this, &rhs);
    	return *this;
    }

    // Sub rhs from this
    Vector2D& operator -=( const Vector2D& rhs ){
    	vector2d_Subtract(this, &rhs);
    	return *this;
    }

    // Scale
    Vector2D& operator *=( double scale ){
    	vector2d_Scale(this, scale);
    	return *this;
    }

    const Vector2D operator+(const Vector2D &other) const {
        return Vector2D(*this) += other;
     }
    const Vector2D operator-(const Vector2D &other) const {
        return Vector2D(*this) -= other;
     }
    const Vector2D operator*(double scale) const {
        return Vector2D(*this) *= scale;
     }

    double length(void) const{
    	return vector2d_Length(this);
    }

    double length2(void) const{
    	return vector2d_LengthSquared(this);
    }

	Vector2D& set( double x, double y){
		vector2d_Set(this,x,y);
		return *this;
	}

	Vector2D& normalise(void){
		vector2d_Normalise(this,this);
		return *this;
	}

	double dot(const Vector2D& other) const{
		return vector2d_DotProduct(this, &other);
	}


	double radians(const Vector2D& other) const{
		return vector2d_AngleRadians(this, &other);
	}

	void dump(Stream& stream, uint8_t decPlaces=2) const{
		stream.print(x, decPlaces);
		stream.print(',');
		stream.print(y, decPlaces);
	}

protected:
	boolean equals(const Vector2D& c) const{
		return (this->x == c.x && this->y == c.y) ? TRUE : FALSE;
	}
private:
};
#endif

#endif /* VECTOR2D_H_ */
