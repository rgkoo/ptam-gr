#ifndef __GESTUREGAME_H
#define __GESTUREGAME_H


#include "ARGame.h"

class GestureGame
{
public:
	virtual void DrawStuff(Vector<3> v3CameraPos){}
	virtual void Reset(){}
	virtual void Init(){}
};

#endif