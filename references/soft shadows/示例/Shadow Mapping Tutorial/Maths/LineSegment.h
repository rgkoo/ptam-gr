#ifndef __LINESEGMENT_H_
#define __LINESEGMENT_H_

#include "Vector3D.h"

class PLANE;

class LineSegment
{
public:
	LineSegment(void){}
	virtual ~LineSegment(void){}
	LineSegment(VECTOR3D& start, VECTOR3D& end):mStart(start), mEnd(end){}

	VECTOR3D& getStartPoint(){
		return mStart;
	}

	VECTOR3D& getEndPoint(){
		return mEnd;
	}

	VECTOR3D getDirection(){
		return mEnd - mStart;
	}

	VECTOR3D getIntersectPoint(PLANE& plane);

protected:
	VECTOR3D mStart, mEnd;
};

#endif