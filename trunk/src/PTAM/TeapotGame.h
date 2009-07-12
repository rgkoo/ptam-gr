#ifndef __TEAPOT_H
#define __TEAPOT_H
#include <TooN/numerics.h>
#include "OpenGL.h"
#include "ARGame.h"
#include "Dog.h"

#include "GestureGame.h"



class TeapotGame : public ARGame, public GestureGame
{
public:
	TeapotGame();
	void DrawStuff(Vector<3> v3CameraPos);
	void Reset();
	void Init();

	void onGesture(GestureData& gesture);


protected:
	bool mbInitialised;
	void DrawTeapot();
	void MakeShadowTex();
	void DrawDog(int frame1, int frame2, float pol);
	void SetVirtualTarget();
	float DisBetweenDogAndTarget();
	float CalAngelDiff();
	void Turn(GoType type);
	bool GotoTargetAnimation(GoType type);
	bool ActionAnimation();
	void RenderFrame();

	GLuint mnShadowTex;
	GLuint mnEyeDisplayList;
	SE3 ase3WorldFromEye;
	int mnFrameCounter;

};


#endif