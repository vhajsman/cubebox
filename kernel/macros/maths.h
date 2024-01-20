#ifndef _MATHS_H
#define _MATHS_H

#include "arrays.h"

#define MAX(ARR, Y)		ARRAY_MAX_INDEX(ARR, Y)
#define MIN(ARR, Y)		ARRAY_MIN_INDEX(ARR, Y)
#define AVR(ARR, Y)		ARRAY_AVERAGE(ARR, Y)

#define PI 3.14159265358979323846
#define E  2.71828182845904523536

#define SQUARE(X)			(X * X)
#define CUBE(X)				(X * X * X)

#define C2(A, B)			(SQUARE(A) + SQUARE(B2))


#define ABS(X)				((X) < 0 ? -(X) : (X))
// #define _SQRT_H(X, C, P)	((ABS(C - P) < 1e-7) ? C : _SQRT_H)

#define O(A, B)				((A + B) * 2)
#define S(A, B)				(A * B)
#define VOLUME(A, B, C)		(S(A, B) * C)

#endif
