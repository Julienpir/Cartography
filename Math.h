#include <math.h>

#ifndef __Math_h__
#define __Math_h__

struct triple {
	double first;
	double second;
	double third;
};


void GPS_to_XY(float lx, float ly,float &x, float &y);


float norm(float x1, float x2);

float angleVec(triple &vec3, triple &vec4);


triple produitVec(triple &vec1, triple &vec2);

#endif