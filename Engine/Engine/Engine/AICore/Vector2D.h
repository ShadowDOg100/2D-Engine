#pragma once

#include <math.h>

const float epsilon = .00001f;

class Vector2D
{
public:

	Vector2D();
	Vector2D( const Vector2D& ) ;	// copy constructor
	Vector2D( float element[2] ) ; // initialize with an array
	Vector2D( double x, double y ) ;

	float& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	float  operator [] ( int index ) const ;	// use to read elements from const vectors

	float Length() ;
	void   Normalize() ;	// modifies the vector to be unit length
	Vector2D Normalized() const ; // returns a new vector that is unit length

	// float-Vector product
	Vector2D operator  - ( void ) const ; // return negative of a vector
	void     operator *= ( float ) ;
	void     operator /= ( float ) ;
	Vector2D operator * ( float ) const ; // for the case when the operand order is Vector * float
	Vector2D operator / ( float ) const ; // for the case when the operand order is Vector * float
	friend Vector2D operator * ( float, Vector2D& ) ; // for the case when the operand order is float * Vector

	// vector addition
	Vector2D operator +  ( Vector2D& ) const ;
	Vector2D operator -  ( Vector2D& ) const ;
	void     operator += ( Vector2D& ) ;
	void     operator -= ( Vector2D& ) ;

	// the Dot-Product
	float   operator *  ( Vector2D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector2D Orthogonal() const ;

	bool     operator == ( const Vector2D& ) const ;
	bool     operator != ( const Vector2D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector2D& Zero_Vector() ;


protected:
	static const int dimension = 2 ;
	float v[dimension] ;
	static const int X = 0;
	static const int Y = 1;
};
