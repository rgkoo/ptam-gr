#include "LineSegment.h"
#include "PLANE.h"

VECTOR3D LineSegment::getIntersectPoint( PLANE& plane )
{
	//TODO:if not intersect!!
	VECTOR3D p;
	float a = plane.a;
	float b = plane.b;
	float c = plane.c;
	float d = plane.d;

	float p1x = mStart.x;
	float p1y = mStart.y;
	float p1z = mStart.z;
	float p2x = mEnd.x;
	float p2y = mEnd.y;
	float p2z = mEnd.z;

	//a * ( p1x + t * (p2x - p1x)) + b * (p1y + t * (p2y - p1y)) + c * (p1z + t * (p2z - p1z)) + d = 0
	float t  = (- a *  p1x - b * p1y - c * p1z - d) / ( a * (p2x - p1x) + b * (p2y - p1y) + c * (p2z - p1z) );
	p.x = p1x + t * (p2x - p1x);
	p.y = p1y + t * (p2y - p1y);
	p.z = p1z + t * (p2z - p1z);
	return p;
}