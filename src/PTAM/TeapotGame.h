#ifndef __TEAPOT_H
#define __TEAPOT_H
#include <TooN/numerics.h>
#include "OpenGL.h"

#include "ARGame.h"
#include "Dog.h"

#include "GestureGame.h"


class ARDriver;

class TeapotGame : public ARGame, public GestureGame
{
public:
	TeapotGame();
	void DrawStuff(Matrix<4>& UFBLinearFrustumMatrix, SE3& cameraSE3FromWorld);
	void DrawStuff(Vector<3> v3CameraPos);
	void Draw2DStuff();
	void Reset();
	void Init();

	void onGesture(GestureData& gesture);


protected:
	bool InitShadowMap(Matrix<4>& UFBLinearFrustumMatrix, SE3& cameraSE3FromWorld);
	void renderShadowedScene(Matrix<4>& UFBLinearFrustumMatrix, SE3& cameraSE3FromWorld);
	void DrawScene();
	
	// Get camera to viewport ray


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
	void RenderPlaneGrids();
	void RenderTargetMarker();
	void RenderLightMarker();
	void RenderFrame();

	GLuint mnShadowTex;
	GLuint mnEyeDisplayList;
	SE3 ase3WorldFromEye;
	int mnFrameCounter;

};


#endif