
#ifndef __GESTUREDATA_H
#define __GESTUREDATA_H

#include <TooN/numerics.h>

struct GestureData{
	Vector<3> mCenterPosition;

	int left,top,width,height;
	bool mbIsValid;	//validity of the gesture
};

#endif