



#include "TeapotGame.h"
#include "OpenGL.h"
#include <gl/glut.h>
#include <cvd/convolution.h>
#include "md2.h"
#include "tga.h"


#include "Maths/Maths.h"




//Camera & light positions
VECTOR3D cameraPosition(-2.5f, 3.5f,-2.5f);
VECTOR3D lightPosition(2.0f, 3.0f,-2.0f);

//Size of shadow map
const int shadowMapSize=512;


//Textures
GLuint shadowMapTexture;

//window size
int windowWidth = 640, windowHeight = 480;

//Matrices
MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;

float angle;
void DrawScene();

//Called for initiation
bool InitShadowMap(void)
{
	//Check for necessary extensions
	if(!GLEW_ARB_depth_texture || !GLEW_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}

	//Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	//Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	//Calculate & save matrices
	glPushMatrix();

	//Save camera projection matrix( original matrix was created on model view matrix stack)
	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth/windowHeight, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	//Save camera view matrix
	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	//Save light projection matrix, note that near plane and far plane are close to get better precision, and that aspect ratio is set to 1 
	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	//Save light view matrix
	glLoadIdentity();
	gluLookAt(	lightPosition.x, lightPosition.y, lightPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	return true;
}


void renderShadowedScene(){
	angle++;

	InitShadowMap();

	//First pass - from light's point of view
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Color buffer not cleared - need to show camera image
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);

	//Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0, 0, 0, 0);

	//Draw the scene
	DrawScene();

	//Read the depth buffer into the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

	//restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);




	//2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);

	glViewport(0, 0, windowWidth, windowHeight);

	//Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT, white*0.2f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white*0.2f);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	DrawScene();


	//3rd pass
	//Draw with bright light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	//Calculate texture matrix for projection
	//This matrix takes us from eye space to the light's clip space
	//It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);	//bias from [-1, 1] to [0, 1]
	MATRIX4X4 textureMatrix=biasMatrix*lightProjectionMatrix*lightViewMatrix;

	//Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	//Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	//Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	DrawScene();

	//Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	//Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);

}








using namespace CVD;

double mdEyeRadius;
double mdShadowHalfSize;

static unsigned char *pixels;
static unsigned int tex_id;
static md2_model_t* dogmodel;
static void LoadTexture(char* filename,bool bFlip)
{
	GLuint id = 0;				//纹理代号

	if(pixels)
	{
		delete[] pixels;
		pixels = NULL;
	}		//用于纹理映射的象素点

	int width, height;

	if(LoadFileTGA(filename,&pixels,&width,&height,bFlip)>0)
	{
		glGenTextures(1,&tex_id);
		glBindTexture(GL_TEXTURE_2D,tex_id);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	}
	else
	{
		std::cout<<"RoleTexture: fail to load new texture:"<<filename<<std::endl;
	}	
}
static void LoadDogModel(char* filename)
{
	printf("Initializing Dog's model...");
	dogmodel=md2_readModel(filename);
	if(!dogmodel){
		printf("failed.\n");
		exit(1);
	}
}

#define SCREEN_LEFT_X -2
#define SCREEN_BOTTOM_Y -2
#define SCREEN_RIGHT_X 2
#define SCREEN_TOP_Y 2


#define Walk_Start_Frame 13
#define Walk_End_Frame 32
#define Run_Start_Frame 33
#define Run_End_Frame 53
#define Bored_Start_Frame 212
#define Bored_End_Frame 379
#define Bored2_Start_Frame 54
#define Bored2_End_Frame 93
#define Bored3_Start_Frame 94
#define Bored3_End_Frame 173
#define Eat_Start_Frame 380
#define Eat_End_Frame 408
#define Ball_Start_Frame 174
#define Ball_End_Frame 211

float Walk_Speed=0.02;
float Run_Speed=0.05;

float Target_Radius=0.075;

Dog* dog;

int cur_go_frame=Walk_Start_Frame;
int cur_action_frame=Bored_Start_Frame;
bool is_idle_finished=false;
int startframe,endframe;

#define IDLETYPE_TURN 0
#define IDLETYPE_WALKTOVIRTUALMARKER 1
#define IDLETYPE_BORED 2

#define TURN_CLOCKWISE 1
#define TURN_COUNTERCLOCKWISE -1

int lastIdleType=IDLETYPE_TURN;  //0:turn, 1:walk to virtual marker 2:bored
int turndirection=TURN_CLOCKWISE; //逆时针为1，顺时针为-1

int repeattime=5;  //吃东西重复几次

int randomtest=0;

GLUquadricObj *quadratic;

float radius=0.01;
float offset=radius+0.02;
float interval=0.02+2*radius;
float height=0.05;
int numx=(SCREEN_RIGHT_X-0.15-SCREEN_LEFT_X)/interval;
static int numy=(SCREEN_TOP_Y-0.12-SCREEN_BOTTOM_Y)/interval;
static float leftx=SCREEN_LEFT_X+offset+0.02, rightx=leftx+numx*interval,
bottomy=SCREEN_BOTTOM_Y+offset, topy=bottomy+numy*interval;
 
template<class T>
static T square(T t)
{
	return t*t;
}

static void GetRadomPosition(float &px, float &py, float &pz, float thresh)
{
#define SCREEN_MIN_X (leftx+0.02)
#define SCREEN_MAX_X (rightx-0.02)
#define SCREEN_MIN_Y (bottomy+0.02)
#define SCREEN_MAX_Y (topy-0.02)
	do 
	{
		px=SCREEN_MIN_X+((float)rand())/RAND_MAX*(SCREEN_MAX_X-SCREEN_MIN_X);
		pz=0;
		py=SCREEN_MIN_Y+((float)rand())/RAND_MAX*(SCREEN_MAX_Y-SCREEN_MIN_Y);
	} while (square(px-dog->position[0])+square(py-dog->position[1])<square(thresh));
}

static float CalDistance(float p1[], float p2[])
{
	float d1=p1[0]-p2[0];
	float d2=p1[1]-p2[1];
	float d3=p1[2]-p2[2];

	return sqrtf(d1*d1+d2*d2+d3*d3);
}


static float dogsize=0.01;

void TeapotGame::DrawDog(int frame1, int frame2, float pol)
{
	glPushMatrix();
	
	glTranslatef(dog->position[0],dog->position[1],dog->position[2]);
	//std::cout<<dog->position[0]<<dog->position[1]<<dog->position[2]<<std::endl;
	float theta=atan2f(dog->direction[1],dog->direction[0])*180.0f/PI;
	glRotatef(90,1,0,0);
	glRotatef(90+theta,0,1,0);


	glEnable(GL_LIGHTING);

	glPushMatrix();
	glScalef(dogsize,dogsize,dogsize);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	md2_drawModel(dogmodel,frame1,frame2,pol);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	///*Very fake shadow*/
	//glDisable(GL_LIGHTING);
	////glLoadIdentity();
	//glRotatef(90,1,0,0);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, mnShadowTex);
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glColor4f(0,0,0,0.5);
	//glBegin(GL_QUADS);
	//glTexCoord2f(0.0,0.0);
	//glVertex2d(-mdShadowHalfSize, -mdShadowHalfSize);
	//glTexCoord2f(0.0,1);
	//glVertex2d(-mdShadowHalfSize,  mdShadowHalfSize);
	//glTexCoord2f(1,1);
	//glVertex2d( mdShadowHalfSize,  mdShadowHalfSize);
	//glTexCoord2f(1,0.0);
	//glVertex2d( mdShadowHalfSize, -mdShadowHalfSize);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);



	//glScalef(1/dogsize,1/dogsize,1/dogsize);
	glPopMatrix();

}

void TeapotGame::SetVirtualTarget()
{
	Marker* virtualmarker = &dog->targetmarker;
	GetRadomPosition(virtualmarker->X,virtualmarker->Y,virtualmarker->Z,0.1);
}


float TeapotGame::DisBetweenDogAndTarget()
{
	//计算狗和物体之间距离
	float temptarget[3];
	temptarget[0]=dog->targetmarker.X;
	temptarget[1]=dog->targetmarker.Y;
	temptarget[2]=dog->targetmarker.Z;

	return CalDistance(dog->position,temptarget);
}

float TeapotGame::CalAngelDiff()
{
	//计算狗的朝向和狗物连线的夹角,0-180，不考虑y值,同时把转向算出
	float dogx=dog->position[0];
	float dogy=dog->position[1];
	float targetx=dog->targetmarker.X;
	float targety=dog->targetmarker.Y;

	float tdx=targetx-dogx;
	float tdy=targety-dogy;

	float dirx=dog->direction[0];
	float diry=dog->direction[1];
	float re=tdx*diry-tdy*dirx;
	if(re>0)
		turndirection=TURN_COUNTERCLOCKWISE;
	else
		turndirection=TURN_CLOCKWISE;  //转向

	float up=tdx*dirx+tdy*diry;
	float down=sqrtf(tdx*tdx+tdy*tdy)*sqrtf(dirx*dirx+diry*diry);

	if(abs(up-down)<0.001)
		return 0;

	float result=acosf(up/down)*180/PI;
	return result;
}

void TeapotGame::Turn(GoType type)
{
	//绕（0，1，0）旋转一个单位角度10,前进一个单位，重新设置狗的朝向和位置
	float speed;
	if(type==By_Walk)
	{
		startframe=Walk_Start_Frame;
		endframe=Walk_End_Frame;	
		speed=Walk_Speed;
	}else
	{
		startframe=Run_Start_Frame;
		endframe=Run_End_Frame;
		speed=Run_Speed;
	}

	if(cur_go_frame==endframe+1)
		cur_go_frame=startframe;

	float cosangle=cosf(turndirection*10*PI/180);
	float sinangle=sinf(turndirection*10*PI/180);
	float x0=dog->direction[0];
	float y0=dog->direction[1];
	dog->direction[0]=x0*cosangle-y0*sinangle;
	dog->direction[1]=x0*sinangle+y0*cosangle;

	dog->position[0]+=dog->direction[0]*speed;
	dog->position[1]+=dog->direction[1]*speed;

	DrawDog(cur_go_frame,endframe,0);
	cur_go_frame+=1;
	

	//cout<<"After turn..."<<CalAngelDiff()<<endl;
}

bool TeapotGame::GotoTargetAnimation(GoType type)
{
	float speed;
	if(type==By_Walk)
	{
		startframe=Walk_Start_Frame;
		endframe=Walk_End_Frame;
		speed=Walk_Speed;
	}else
	{
		startframe=Run_Start_Frame;
		endframe=Run_End_Frame;
		speed=Run_Speed;
	}

	//距离物一定距离返回true	
	if(DisBetweenDogAndTarget()<Target_Radius)
	{
		if(cur_go_frame==endframe+1)
			return true;

		speed=0; //跑完一套帧数
	}

	//否则按照当前朝向移动一个距离
	//跑步33-53
	if(cur_go_frame==endframe+1)
		cur_go_frame=startframe;

	for(int i=0; i<3; i++)
		dog->position[i]+=dog->direction[i]*speed;

	DrawDog(cur_go_frame,endframe,0);
	cur_go_frame+=1;

	printf("finish...\n");
	return false;
}



static void SetAnimation(ActionType type)
{

	if (rand()<RAND_MAX/3)
	{
		startframe=Bored_Start_Frame;
		endframe=Bored_End_Frame;
		cur_action_frame=Bored_Start_Frame;
	}
	else if (rand()<RAND_MAX*2/3)
	{
		startframe=Bored2_Start_Frame;
		endframe=Bored2_End_Frame;
		cur_action_frame=Bored2_Start_Frame;
	}
	else 
	{
		startframe=Bored3_Start_Frame;
		endframe=Bored3_End_Frame;
		cur_action_frame=Bored3_Start_Frame;
	}
}

bool TeapotGame::ActionAnimation()
{	
	if(cur_action_frame==endframe+1){		
		cur_action_frame=startframe;
		return true;
	}
	DrawDog(cur_action_frame,endframe,0);
	cur_action_frame++;
	return false;
}

void TeapotGame::RenderFrame()
{
	//根据狗的位置和朝向来回走
	//0:turn, 1:walk to virtual marker 2:bored



	if(lastIdleType==IDLETYPE_TURN)
	{  //turn
		float an;
		if((an=CalAngelDiff())>10)
		{		
			Turn(By_Walk);
			lastIdleType=IDLETYPE_TURN;			
		}else
		{
			float targetx=dog->targetmarker.X-dog->position[0];
			float targety=dog->targetmarker.Y-dog->position[1];
			float tmp=sqrtf(targetx*targetx+targety*targety);

			dog->direction[0]=targetx/tmp;
			dog->direction[1]=targety/tmp;
			DrawDog(Walk_End_Frame,Walk_End_Frame,0);
			lastIdleType=IDLETYPE_WALKTOVIRTUALMARKER;  //begin to walk to target			
		}
		is_idle_finished=true;	
		return;
	}else if(lastIdleType==IDLETYPE_WALKTOVIRTUALMARKER)
	{
		if(GotoTargetAnimation(By_Walk))
		{
			lastIdleType=IDLETYPE_BORED;
			SetAnimation(Action_Bored);			
			DrawDog(Walk_End_Frame,Walk_End_Frame,0);
		}
		is_idle_finished=true;
		return;
	}else
	{
		if(ActionAnimation())
		{
			SetVirtualTarget();
			lastIdleType=IDLETYPE_TURN;
			cur_go_frame=Walk_Start_Frame;
			DrawDog(Walk_End_Frame,Walk_End_Frame,0);
			is_idle_finished=true;		
			return;
		}
		is_idle_finished=false;
		return;
	}
}

TeapotGame::TeapotGame()
{
	mdEyeRadius = 0.1;
	mdShadowHalfSize = 2.5 * mdEyeRadius;
	mbInitialised = false;
}

void TeapotGame::DrawStuff(Vector<3> v3CameraPos)
{
	if(!mbInitialised)
		Init();

	mnFrameCounter ++;

	renderShadowedScene();



	//render unshadowed scene()

	////glDisable(GL_BLEND);
	////glEnable(GL_CULL_FACE);
	////glFrontFace(GL_CW);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);

	////set up light and material
	//GLfloat af[4]; 
	//af[0]=0.3; af[1]=0.3; af[2]=0.3; af[3]=1.0;
	//glLightfv(GL_LIGHT0, GL_AMBIENT, af);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, af);
	//af[0]=1.0; af[1]=0.0; af[2]=1.0; af[3]=0.0;
	//glLightfv(GL_LIGHT0, GL_POSITION, af);
	//af[0]=1.0; af[1]=1.0; af[2]=1.0; af[3]=1.0;
	//glMaterialfv(GL_FRONT, GL_SPECULAR, af);
	//glMaterialf(GL_FRONT, GL_SHININESS, 50.0);



	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glScaled(mdEyeRadius, mdEyeRadius, mdEyeRadius);
	//
	////glCallList(mnEyeDisplayList);
	//DrawTeapot();
	
};


void TeapotGame::Reset()
{
	ase3WorldFromEye = SE3();
	mnFrameCounter = 0;
};


void TeapotGame::DrawTeapot()
{
	RenderLightMarker();
	RenderPlaneGrids();
	RenderTargetMarker();

	RenderFrame();
//  	glPushMatrix();
//  		glRotated(90,1,0,0);
// 		glTranslated(0,1,0);
// 		GLfloat mat_teapot1[] = {0.34f, 0.129f, 0.3f, 1.0f };
//  		//glColor3fv(mat_teapot1);
// 		glColor3f(0.4f, 0.26f, 0.3f);
//  		glutSolidTeapot(1.0);
//  	glPopMatrix();	
}

void TeapotGame::Init()
{
	if(mbInitialised) return;
	mbInitialised = true;
	// Set up the display list for the eyeball.
	//mnEyeDisplayList = glGenLists(1);
// 	glNewList(mnEyeDisplayList,GL_COMPILE);
// 	DrawTeapot();
// 	glEndList();
	MakeShadowTex();
	LoadTexture("Data/dog.tga", true);
	LoadDogModel("Data/dog.md2");
	dog=new Dog();
	SetVirtualTarget();
	//glutFullScreen();


	quadratic = gluNewQuadric();				// 创建二次几何体
	gluQuadricNormals(quadratic, GLU_SMOOTH);		// 使用平滑法线
	gluQuadricTexture(quadratic, GL_TRUE);		// 使用纹理

	if(! InitShadowMap())	
		exit(0);
};


void TeapotGame::MakeShadowTex()
{
	const int nTexSize = 256;
	Image<CVD::byte> imShadow(ImageRef(nTexSize, nTexSize));
	double dFrac = 1.0 - mdEyeRadius / mdShadowHalfSize;
	double dCenterPos = dFrac * nTexSize / 2 - 0.5;
	ImageRef irCenter; irCenter.x = irCenter.y = nTexSize/2;//(int) dCenterPos;
	int nRadius = int (nTexSize / 2  * 0.85);
	unsigned int nRadiusSquared = nRadius*nRadius;
	ImageRef ir;
	for(ir.y = 0; ir.y < nTexSize; ir.y++)
		for(ir.x = 0; ir.x < nTexSize; ir.x++)
		{
			CVD::byte val = 0;
			if((ir - irCenter).mag_squared() < nRadiusSquared)
				val = 255;
			imShadow[ir] = val;
			// 			imShadow[ImageRef(nTexSize - 1 - ir.x, ir.y)] = val;
			// 			imShadow[ImageRef(nTexSize - 1 - ir.x, nTexSize - 1 - ir.y)] = val;
			// 			imShadow[ImageRef(ir.x, nTexSize - 1 - ir.y)] = val;
		}

		convolveGaussian(imShadow, 4.0);
		glGenTextures(1, &mnShadowTex);
		glBindTexture(GL_TEXTURE_2D,mnShadowTex);
		glTexImage2D(GL_TEXTURE_2D, 0, 
			GL_ALPHA, nTexSize, nTexSize, 0, 
			GL_ALPHA, GL_UNSIGNED_BYTE, imShadow.data()); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};

void TeapotGame::onGesture( GestureData& gesture )
{

}

void TeapotGame::RenderPlaneGrids()
{
	glColor3f(1.0f,1.0f,1.0f);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	for(float x = SCREEN_MIN_X; x <= SCREEN_MAX_X; x += (SCREEN_MAX_X-SCREEN_MIN_X)/20){
		glVertex3f(x, SCREEN_MIN_Y, 0);
		glVertex3f(x, SCREEN_MAX_Y, 0);
	}
	for(float y = SCREEN_MIN_Y; y <= SCREEN_MAX_Y; y += (SCREEN_MAX_Y-SCREEN_MIN_Y)/20){
		glVertex3f(SCREEN_MIN_X, y, 0);
		glVertex3f(SCREEN_MAX_X, y, 0);
	}
	glEnd();
	//glBegin(GL_QUADS);
	//glVertex3f(SCREEN_MIN_X, SCREEN_MIN_Y, 0);
	//glVertex3f(SCREEN_MAX_X, SCREEN_MIN_Y, 0);
	//glVertex3f(SCREEN_MAX_X, SCREEN_MAX_Y, 0);
	//glVertex3f(SCREEN_MIN_X, SCREEN_MAX_Y, 0);
	//glEnd();
	glEnable(GL_LIGHTING);
}

void TeapotGame::RenderTargetMarker()
{
	glPushMatrix();

	glDisable(GL_LIGHTING);
	//red ball 
	glColor3f(1.0f,0.0f,0.0f);
	//move to target marker
	glTranslatef(dog->targetmarker.X, dog->targetmarker.Y, dog->targetmarker.Z);
	//draw sphere
	gluSphere(quadratic,0.1f,32,32);
	glEnable(GL_LIGHTING);

	glPopMatrix();

}

void TeapotGame::RenderLightMarker()
{
	glPushMatrix();

	glDisable(GL_LIGHTING);
	//red ball 
	glColor3f(1.0f,1.0f,0.0f);
	//move to target marker
	glTranslatef(1.0, 1.0, 1.0);
	//draw sphere
	gluSphere(quadratic,0.1f,8,8);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}


void DrawScene()
{
	//Display lists for objects
	static GLuint spheresList=0, torusList=0, baseList=0;

	//Create spheres list if necessary
	if(!spheresList)
	{
		spheresList=glGenLists(1);
		glNewList(spheresList, GL_COMPILE);
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glPushMatrix();

			glTranslatef(0.45f, 1.0f, 0.45f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(-0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.0f, 0.0f,-0.9f);
			glutSolidSphere(0.2, 24, 24);

			glTranslatef(0.9f, 0.0f, 0.0f);
			glutSolidSphere(0.2, 24, 24);

			glPopMatrix();
		}
		glEndList();
	}

	//Create torus if necessary
	if(!torusList)
	{
		torusList=glGenLists(1);
		glNewList(torusList, GL_COMPILE);
		{
			glColor3f(1.0f, 0.0f, 0.0f);
			glPushMatrix();

			glTranslatef(0.0f, 0.5f, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glutSolidTorus(0.2, 0.5, 24, 48);

			glPopMatrix();
		}
		glEndList();
	}

	//Create base if necessary
	if(!baseList)
	{
		baseList=glGenLists(1);
		glNewList(baseList, GL_COMPILE);
		{
			glColor3f(0.0f, 0.0f, 1.0f);
			glPushMatrix();

			glScalef(1.0f, 0.05f, 1.0f);
			glutSolidCube(3.0f);

			glPopMatrix();
		}
		glEndList();
	}


	//Draw objects
	glCallList(baseList);
	glCallList(torusList);

	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glCallList(spheresList);
	glPopMatrix();
}

