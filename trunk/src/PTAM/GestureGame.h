
#ifndef __GESTUREGAME_H
#define __GESTUREGAME_H

#include "ARGame.h"


/**
* GestureGame is a subclass of ARGame,
* which implements a virtual 3d-game by gesture recognition.
*/
class GestureGame:public ARGame{
public:
	GestureGame(){}
	void DrawStuff(Vector<3> v3CameraPos){}
	void Reset(){}
	void Init(){}
};

#endif