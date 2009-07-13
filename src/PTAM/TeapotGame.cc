#include "TeapotGame.h"
#include "OpenGL.h"
#include <gl/glut.h>
#include <cvd/convolution.h>
#include "md2.h"
#include "tga.h"

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

int lastIdleType=IDLETYPE_TURN;  //0:turn, 1:walk to virtual marker 2:bored
int turndirection=1; //逆时针为1，顺时针为-1

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

	glPushMatrix();
	glScalef(dogsize,dogsize,dogsize);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	md2_drawModel(dogmodel,frame1,frame2,pol);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glDisable(GL_LIGHTING);

	//glLoadIdentity();
	glRotatef(90,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mnShadowTex);
	glEnable(GL_BLEND);
	glColor4f(0,0,0,0.1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0);
	glVertex2d(-mdShadowHalfSize, -mdShadowHalfSize);
	glTexCoord2f(0.0,1);
	glVertex2d(-mdShadowHalfSize,  mdShadowHalfSize);
	glTexCoord2f(1,1);
	glVertex2d( mdShadowHalfSize,  mdShadowHalfSize);
	glTexCoord2f(1,0.0);
	glVertex2d( mdShadowHalfSize, -mdShadowHalfSize);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

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
		turndirection=-1;
	else
		turndirection=1;  //转向

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

	//glDisable(GL_BLEND);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	//set up light and material
	GLfloat af[4]; 
	af[0]=0.3; af[1]=0.3; af[2]=0.3; af[3]=1.0;
	glLightfv(GL_LIGHT0, GL_AMBIENT, af);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, af);
	af[0]=1.0; af[1]=0.0; af[2]=1.0; af[3]=0.0;
	glLightfv(GL_LIGHT0, GL_POSITION, af);
	af[0]=1.0; af[1]=1.0; af[2]=1.0; af[3]=1.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, af);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glScaled(mdEyeRadius, mdEyeRadius, mdEyeRadius);
	//glCallList(mnEyeDisplayList);
	DrawTeapot();
	
};


void TeapotGame::Reset()
{
	ase3WorldFromEye = SE3();
	mnFrameCounter = 0;
};


void TeapotGame::DrawTeapot()
{

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