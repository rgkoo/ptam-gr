#pragma once

#include "Vector3D.h"

class LineSegment
{
public:
	LineSegment(void);
	virtual ~LineSegment(void);
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

protected:
	VECTOR3D mStart, mEnd;
};
