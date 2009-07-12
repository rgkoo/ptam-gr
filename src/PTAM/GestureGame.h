#ifndef __GESTUREGAME_H
#define __GESTUREGAME_H

#include "GestureData.h"

/**
* Gesture Game interface.
*/
class GestureGame{
public:
	virtual void onGesture(GestureData& gesture) = 0;	// do something upon specified gesture.
};


#endif