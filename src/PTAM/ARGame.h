#ifndef __ARGAME_H
#define __ARGAME_H
#include <TooN/numerics.h>
#include "OpenGL.h"

/**
* ARGame interface.
*/
class ARGame
{
public:
	virtual void DrawStuff(SE3 cameraSE3FromWorld) = 0;
	virtual void DrawStuff(Vector<3> v3CameraPos) = 0;
	virtual void Reset() = 0;
	virtual void Init() = 0;
};


#endif