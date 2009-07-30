#ifndef __PLANE_H_
#define __PLANE_H_

#include "LineSegment.h"

/**Ax+By+Cz+D=0*/

class Plane{
public:
	Plane(float _a, float _b, float _c, float _d):a(_a), b(_b), c(_c), d(_d){}
protected:
	float a, b, c, d;
};

#endif