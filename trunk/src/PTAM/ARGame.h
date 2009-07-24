#ifndef __ARGAME_H
#define __ARGAME_H
#include <TooN/numerics.h>
#include "OpenGL.h"

class ARDriver;

/**
* ARGame interface.
*/
class ARGame
{
public:
	/**
	* Draw stuff according to camera matrix.
	*/
	virtual void DrawStuff(Matrix<4>& UFBLinearFrustumMatrix, SE3& cameraSE3FromWorld) = 0;
	/**
	@Deprecated
	*/
	virtual void DrawStuff(Vector<3> v3CameraPos) = 0;
	virtual void Draw2DStuff(){}

	virtual void Reset() = 0;
	virtual void Init() = 0;
public:
	ARDriver* mARDriver;
};


#endif