/*
 * shadow.c - Shadow mapping demo with 4-samples PCF
 *
 * Simple test program for OpenGL and glX
 * Copyright (c) 2007 Cesare Tirabassi <norsetto@ubuntu.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define _GNU_SOURCE

/*
 * HEADERS
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <sys/time.h>
#include <assert.h>

#include "image.h"
#include "extension.h"
#include "shaders.h"
#include "md2.h"
#include "camera.h"
#include "common.h"

/*
 * Globals
 */

static GLboolean full_screen = GL_FALSE;
static GLboolean print_fps = GL_FALSE;
static GLboolean print_info = GL_FALSE;
static GLboolean enable_fsaa = GL_FALSE;
static GLboolean enable_anisotropic = GL_FALSE;
static GLboolean windowpos = GL_FALSE;
static GLboolean wireframe = GL_FALSE;
static GLboolean GLSLshader = GL_TRUE;
static GLboolean use_timer = GL_FALSE;

GLint width, height;

/* Model variables */

//Texture image data
GLuint *texture_id; //Pointer to texture objects
GLfloat MaxAnisoLevel,AnisoLevel;
const int NUM_TEXTURES = 8; //2 * # of md2 models + 2 (floor)

//Ground parameters
IMAGE_PTR floor_texture = NULL;
GLfloat fExtent = 100.0f;
GLfloat fStep = 25.0f;
GLfloat y_floor = 0.0f;
static GLuint FList;

//MD2 model variable
#define NUM_MODELS 3
MD2_MODEL_PTR md2_model[NUM_MODELS];
char *filename[] = {	DATA_DIR "data/ogros/Ogros.md2", DATA_DIR "data/ogros/ogros.tga", DATA_DIR "data/ogros/normal1.tga",
			DATA_DIR "data/ogros/Weapon.md2", DATA_DIR "data/ogros/Weapon.tga", DATA_DIR "data/ogros/normal2.tga",
			DATA_DIR "data/drfreak/drfreak.md2", DATA_DIR "data/drfreak/drfreak.tga", DATA_DIR "data/drfreak/normal.tga",
			DATA_DIR "data/floor/floor.bmp", DATA_DIR "data/floor/normal.bmp" };

//Shader functions
#define NUM_SHADERS 2
GLuint ids[NUM_SHADERS];	// low-level shader object names
GLhandleARB progObj;		// GLSL program object
GLint colorMap, bumpMap, shadowMap, tangent, binormal;
const GLfloat floorT[] = { 1.0f, 0.0f,  0.0f };//Floor Tangent vector
const GLfloat floorB[] = { 0.0f, 0.0f, -1.0f };//Floor Binormal vector
const GLfloat floorN[] = { 0.0f, 1.0f,  0.0f };//Floor Normal vector

//Framebuffer object
GLuint FBO = 0;		//FrameBuffer Object
GLuint shadow_tx = 0;	//The shadow texture identifier
const GLuint shadow_sz = 1024;	//The shadow map size
GLfloat scalePoly = 10.0f, biasPoly = 0.f;

/* Motion and view variables */
const float tStep = 1000.f;	//Translational step 
const float aStep = 0.001f;	//Rotational step (radians)
const float mouse_scale_t = 0.001f;	//Mouse smoothing translations
const float mouse_scale_r = 0.2f;	//Mouse smoothing rotations

float sinE, cosE, sinA, cosA, fScale;

int old_x, old_y;

float xRot = 0.f, yRot = 0.f;

GLdouble cameraModelview[16], cameraModelviewInverse[16];

/* Light */
const GLfloat lightRadius = 200.0f;
GLfloat lightPos[4];
GLfloat eLit, aLit;		//Light elevation and azimuth angles
const GLfloat lStep = 0.05f;	//Light rotational step
GLdouble lightProjection[16], lightModelview[16];

/* Timer */
struct timeval tv;
double etime, dt;
double t0 = 0.0f, t1 = 0.0f, t2 = 0.0f;
GLuint fps_font;
GLfloat Timed = 5.0f, max_timer = 0.0f;
GLfloat fps_mean = 0.0f, fps_count = 0.0f;

/* Signal handling */
struct sigaction act, oact;
GLint signal_pending = 0;

void signal_handler(GLint sig)
{
	signal_pending = sig;
}

/*
 * Do a fast inversion of a rotation and translation matrix
 *
 */

void FastInvert4( GLdouble mat[16], GLdouble ret[16] )
{
	ret[0 ] = mat[0];
	ret[1 ] = mat[4];
	ret[2 ] = mat[8];
	ret[3 ] = 0.0;
	ret[4 ] = mat[1];
	ret[5 ] = mat[5];
	ret[6 ] = mat[9];
	ret[7 ] = 0.0;
	ret[8 ] = mat[2];
	ret[9 ] = mat[6];
	ret[10] = mat[10];
	ret[11] = 0.0;
	ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
	ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
	ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
	ret[15] = 1.0;
}

/*
 * If we use scaling we need to invert the rotational part
 *
 */

void Invert4( GLdouble mat[16], GLdouble ret[16] )
{
GLdouble det;

	det = mat[0] * mat[5] * mat[10];
	det += mat[4] * mat[9] * mat[2];
	det += mat[8] * mat[1] * mat[6];
	det -= mat[8] * mat[5] * mat[2];
	det -= mat[4] * mat[1] * mat[10];
	det -= mat[0] * mat[9] * mat[6];
	det = 1.0f / det;

	ret[0] = (mat[5] * mat[10] - mat[9] * mat[6]) * det;
	ret[1] = -(mat[1] * mat[10] - mat[9] * mat[2]) * det;
	ret[2] = (mat[1] * mat[6] - mat[5] * mat[2]) * det;
	ret[3] = 0.0f;
	ret[4] = -(mat[4] * mat[10] - mat[8] * mat[6]) * det;
	ret[5] = (mat[0] * mat[10] - mat[8] * mat[2]) * det;
	ret[6] = -(mat[0] * mat[6] - mat[4] * mat[2]) * det;
	ret[7] = 0.0f;
	ret[8] = (mat[4] * mat[9] - mat[8] * mat[5]) * det;
	ret[9] = -(mat[0] * mat[9] - mat[8] * mat[1]) * det;
	ret[10] = (mat[0] * mat[5] - mat[4] * mat[1]) * det;
	ret[11] = 0.0f;
	ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
	ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
	ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
	ret[15] = 1.0f;
}

/*
 * Check frame buffer status
 */

GLboolean CheckFramebufferStatus(void)
{
GLenum status;
GLboolean out = GL_FALSE;

	status = (GLenum) glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		out = GL_TRUE;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		printf("<!> Unsupported framebuffer format\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		printf("<!> Framebuffer incomplete, missing attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
		printf("<!> Framebuffer incomplete, duplicate attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		printf("<!> Framebuffer incomplete, attached images must have same dimensions\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		printf("<!> Framebuffer incomplete, attached images must have same format\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		printf("<!> Framebuffer incomplete, missing draw buffer\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		printf("<!> Framebuffer incomplete, missing read buffer\n");
		break;
	default:
		assert(0);
	}
	return out;
}

/*
 * Create a framebuffer with one depth framebuffer objects
 */

GLboolean CreateFB( GLuint shadow_sz, GLuint *FBO, GLuint *FBOdepth )
{
	if( *FBO )
		glDeleteFramebuffersEXT( 1, FBO  );
	if( *FBOdepth )
		glDeleteTextures( 1, FBOdepth );

	glGenFramebuffersEXT( 1, FBO  );
	glGenTextures( 1, FBOdepth );

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, *FBO );

	//Initialize depth texture
	glBindTexture( GL_TEXTURE_2D, *FBOdepth );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE_ARB );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadow_sz, shadow_sz, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, *FBOdepth, 0);

        //No color buffer to draw to or read from
	glDrawBuffer(GL_FALSE);
	glReadBuffer(GL_FALSE);

	//Check if everything is as it should
	if (!CheckFramebufferStatus())
		return GL_FALSE;

	// Re-enable rendering to the window
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return GL_TRUE;
}

/*
 * change_size
 */

void change_size(GLsizei w, GLsizei h)
{
GLdouble m[4][4];
GLdouble sine, cotangent, deltaZ;
GLdouble radians, fAspect; 
const GLdouble zNear = 1.f, zFar = 1000.f, fovy = 50.f;

	if (h == 0)
		h = 1;

	width = w;
	height = h;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	radians = fovy * PI / 360.f;
	deltaZ = zFar - zNear;
	sine = sin(radians);
	cotangent = cos(radians) / sine;

	m[0][0] = cotangent / fAspect;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;
	m[1][0] = 0.f;
	m[1][1] = cotangent;
	m[1][2] = 0.f;
	m[1][3] = 0.f;
	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = -(zFar + zNear) / deltaZ;
	m[2][3] = -1.f;
	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = -2.f * zNear * zFar / deltaZ;
	m[3][3] = 0.f;

	glMultMatrixd(&m[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * draw floor
 */

void DrawGround(void)
{
float Tex_S_1, Tex_S_2, Tex_T;
float TexRCP = 4.0f / fExtent;
float iStrip, iRun;

	for(iStrip = -fExtent; iStrip < fExtent; iStrip += fStep)
	{
		Tex_S_1 = ( iStrip + fExtent ) * TexRCP;
		Tex_S_2 = ( iStrip + fStep + fExtent ) * TexRCP;

		glBegin(GL_TRIANGLE_STRIP);
			for(iRun = -fExtent; iRun <= fExtent; iRun += fStep)
			{
				Tex_T = ( fExtent - iRun ) * TexRCP;

				glTexCoord2f( Tex_S_1, Tex_T );
                		glVertex3f(iStrip, y_floor, iRun);
				glTexCoord2f( Tex_S_2, Tex_T );
                		glVertex3f(iStrip + fStep, y_floor, iRun);
			}
		glEnd();
	}
}

/*
 * setup
 */

int setup(void)
{
GLuint major, minor;
GLint success;
GLuint model_id;

	srand( (unsigned)time( NULL ) );

	// Make sure required functionality is available!
	if (!getGLversion( &major, &minor))
		return -1;

	if (major < 2)
	{
		printf("<!> OpenGL 2.0 or higher is required\n");
		return -1;
	}

	windowpos = IsExtSupported("GL_ARB_window_pos");

	// Make sure required functionality is available!
	if (!(IsExtSupported("GL_ARB_vertex_program")))
	{
		printf("<!> ARB vertex program extension not supported\n");
		return -1;
	}

	if (!(IsExtSupported("GL_ARB_fragment_program")))
	{
		printf("<!> ARB fragment program extension not supported\n");
		return -1;
	}

	if (!(IsExtSupported("GL_ARB_vertex_shader")))
	{
		printf("<!> ARB vertex shader extension not supported\n");
		return -1;
	}

	if (!(IsExtSupported("GL_ARB_fragment_shader")))
	{
		printf("<!> ARB fragment shader extension not supported\n");
		return -1;
	}

	if (!(IsExtSupported("GL_EXT_framebuffer_object")))
	{
		printf("<!> Framebuffer object extension not supported\n");
		return -1;
	}

	//Define extension prointers
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)glXGetProcAddressARB("glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)glXGetProcAddressARB("glBindFramebufferEXT");
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)glXGetProcAddressARB("glFramebufferTexture2DEXT");
	glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)glXGetProcAddressARB("glGenRenderbuffersEXT");
	glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)glXGetProcAddressARB("glBindRenderbufferEXT");
	glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glXGetProcAddressARB("glRenderbufferStorageEXT");
	glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glXGetProcAddressARB("glFramebufferRenderbufferEXT");
	glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)glXGetProcAddressARB("glDeleteFramebuffersEXT");
	glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)glXGetProcAddressARB("glDeleteRenderbuffersEXT");
	glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glXGetProcAddressARB("glCheckFramebufferStatusEXT");

	if (	!glGenFramebuffersEXT || !glBindFramebufferEXT ||
		!glFramebufferTexture2DEXT || !glGenRenderbuffersEXT ||
		!glBindRenderbufferEXT || !glRenderbufferStorageEXT ||
		!glFramebufferRenderbufferEXT || !glDeleteFramebuffersEXT ||
		!glDeleteRenderbuffersEXT || !glCheckFramebufferStatusEXT )
	{
		printf("<!> Required extension not supported\n");
		return -1;
	}

	//Enable smooth shading
	glShadeModel(GL_SMOOTH);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset( scalePoly, biasPoly );

	//Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace( GL_CW );

	//Background color (Ligth blue)
	glClearColor(0.0f, 0.4f, 0.8f, 1.0f);

	//Generate texture objects
	texture_id = (GLuint *)malloc( NUM_TEXTURES * sizeof(GLuint) );
	glGenTextures( NUM_TEXTURES, texture_id );

	// Enable Anisotropic filtering (for 2D textures only)
	if( enable_anisotropic )
	{
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisoLevel);
		if( AnisoLevel >  MaxAnisoLevel)
			AnisoLevel = MaxAnisoLevel;
		if( print_info )
			printf("<-> %.0fX anisotropic filtering enabled\n", AnisoLevel);
	}
	else
		AnisoLevel = 0.0f;

	// Enable FSAA
	if (enable_fsaa)
		glEnable(GL_MULTISAMPLE);

	//Load MD2 models and textures
	for (model_id = 0; model_id < NUM_MODELS; ++model_id)
	{
		// Load the .md2 model
		if ( (md2_model[model_id] = (MD2_MODEL_PTR)malloc(sizeof(MD2_MODEL))) == NULL)
		{
			printf("<!> Unable to allocate memory for MD2 model in %s\n", filename[3*model_id]);
			return -1;
		}

		if ( !LoadMD2(md2_model[model_id], filename[3*model_id]) )
		{
			printf("<!> Unable to load MD2 model from %s\n", filename[3*model_id]);
			return -1;
		}
		if( print_info )
			printf("<-> Loaded MD2 model from \"%s\" for model #%d\n", filename[3*model_id], model_id);

		// Load texture from disk
		if ( !LoadImageFromDisk(md2_model[model_id]->skin, filename[3*model_id+1]) )
		{
			printf("<!> Unable to load texture from %s \n", filename[3*model_id+1]);
			return -1;
		}
		if( print_info )
			printf("<-> Loaded texture from \"%s\" for model #%d\n", filename[3*model_id+1], model_id);

		//Set up model texture parameters
		glBindTexture(GL_TEXTURE_2D, texture_id[2*model_id]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(	GL_TEXTURE_2D, 0, md2_model[model_id]->skin->components,
				md2_model[model_id]->skin->width, md2_model[model_id]->skin->height,
				0, md2_model[model_id]->skin->format, GL_UNSIGNED_BYTE,
				md2_model[model_id]->skin->data);

		// Load normal from disk
		if ( !LoadImageFromDisk(md2_model[model_id]->normal, filename[3*model_id+2]) )
		{
			printf("<!> Unable to load texture from %s \n", filename[3*model_id+2]);
			return -1;
		}
		if( print_info )
			printf("<-> Loaded texture from \"%s\" for model #%d\n", filename[3*model_id+2], model_id);

		//Set up model texture parameters
		glBindTexture(GL_TEXTURE_2D, texture_id[2*model_id+1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(	GL_TEXTURE_2D, 0, md2_model[model_id]->normal->components,
				md2_model[model_id]->normal->width, md2_model[model_id]->normal->height,
				0, md2_model[model_id]->normal->format, GL_UNSIGNED_BYTE,
				md2_model[model_id]->normal->data);
	}

	// Load floor texture from disk
	floor_texture = (IMAGE_PTR)malloc(sizeof(IMAGE));
	floor_texture->data = NULL;
	if ( !LoadImageFromDisk(floor_texture, filename[3*NUM_MODELS]) )
	{
		printf("<!> Unable to load texture from %s\n", filename[3*NUM_MODELS]);
		return -1;
	}
	if( print_info )
		printf("<-> Loaded texture from \"%s\"\n", filename[3*NUM_MODELS]);
	
	//Set up floor texture parameters
	glBindTexture(GL_TEXTURE_2D, texture_id[NUM_TEXTURES-2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (enable_anisotropic)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, AnisoLevel);

	glTexImage2D(	GL_TEXTURE_2D, 0, floor_texture->components,
			floor_texture->width, floor_texture->height,
			0, floor_texture->format, GL_UNSIGNED_BYTE,
			floor_texture->data);

	if (floor_texture->data)
	{
		free(floor_texture->data);
		floor_texture->data = NULL;
	}

	// Load floor normal map from disk
	floor_texture = (IMAGE_PTR)malloc(sizeof(IMAGE));
	floor_texture->data = NULL;
	if ( !LoadImageFromDisk(floor_texture, filename[3*NUM_MODELS+1]) )
	{
		printf("<!> Unable to load texture from %s\n", filename[3*NUM_MODELS+1]);
		return -1;
	}

	if( print_info )
		printf("<-> Loaded texture from \"%s\"\n", filename[3*NUM_MODELS+1]);
	
	//Set up floor texture parameters
	glBindTexture(GL_TEXTURE_2D, texture_id[NUM_TEXTURES-1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (enable_anisotropic)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, AnisoLevel);

	glTexImage2D(	GL_TEXTURE_2D, 0, floor_texture->components,
			floor_texture->width, floor_texture->height,
			0, floor_texture->format, GL_UNSIGNED_BYTE,
			floor_texture->data);

	if (floor_texture->data)
	{
		free(floor_texture->data);
		floor_texture->data = NULL;
	}

	// Load and compile low-level shaders
	glGenProgramsARB(NUM_SHADERS, ids);
	if (!CompileShader(GL_VERTEX_PROGRAM_ARB,   ids[0], DATA_DIR "data/shaders/light.vp"))
		return -1;
	if (!CompileShader(GL_FRAGMENT_PROGRAM_ARB, ids[1], DATA_DIR "data/shaders/lightMasked.fp"))
		return -1;

	// Create program object and compile GLSL shaders
	progObj = glCreateProgramObjectARB();

	if (!CompileGLSLshaders(&progObj,
		DATA_DIR "data/shaders/bumpTBN_SH_VP.glsl",
		DATA_DIR "data/shaders/bumpTBN_SH_FP.glsl", GL_FALSE))
		return -1;

	// Retrieve uniform and attributes locations
	glUseProgramObjectARB(progObj);

	colorMap = glGetUniformLocationARB(progObj, "colorMap");
	bumpMap = glGetUniformLocationARB(progObj, "bumpMap");
	shadowMap = glGetUniformLocationARB(progObj, "shadowMap");
	tangent = glGetAttribLocation(progObj, "tangent");
	binormal = glGetAttribLocation(progObj, "binormal");

	for (model_id = 0; model_id < NUM_MODELS; ++model_id)
	{
		md2_model[model_id]->tangent = tangent;
		md2_model[model_id]->binormal = binormal;
	}

	// Set texture units
	glUniform1i(  colorMap, 0 );
	glUniform1i(   bumpMap, 1 );
	glUniform1i( shadowMap, 2 );

	//Validate shaders
	glValidateProgramARB(progObj);
	glGetObjectParameterivARB(progObj, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
	if (!success)
	{
		GLbyte infoLog[MAX_INFO_LOG_SIZE];
		glGetInfoLogARB(progObj, MAX_INFO_LOG_SIZE, NULL, (char *)infoLog);
		printf("<!> Error in program validation\n");
		printf("Info log: %s\n", infoLog);
		return -1;
	}

	//Disable GLSL and enable low-level shaders
	glUseProgramObjectARB(0);
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glBindProgramARB(GL_VERTEX_PROGRAM_ARB,   ids[0]);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, ids[1]);

	// Create FrameBuffer
	if (!CreateFB( shadow_sz, &FBO, &shadow_tx ))
		return -1;

	//Prebuild the display lists
	FList = glGenLists(1);
	glNewList(FList, GL_COMPILE);
		DrawGround();
	glEndList();

	//Init animations and kinematic state
	md2_model[0]->anim_state = ANIMATION_RUN;
	md2_model[0]->anim_command = ANIMATION_START;
	md2_model[0]->delta_rate = 30.f;
	md2_model[0]->position.x = 85.f;
	md2_model[0]->position.z = 0.f;
	md2_model[0]->rotation = 90.f;

	md2_model[1]->anim_state = ANIMATION_RUN;
	md2_model[1]->anim_command = ANIMATION_START;
	md2_model[1]->delta_rate = 30.f;
	md2_model[1]->position.x = 85.f;
	md2_model[1]->position.z = 0.f;
	md2_model[1]->rotation = 90.f;

	md2_model[2]->anim_state = ANIMATION_STANDING_IDLE;
	md2_model[2]->anim_command = ANIMATION_START;

	// Set initial camera position and orientation
	xCam = 0.0f;
	yCam = 70.0f;
	zCam = 200.0f;

	eCam = -0.35f;
	aCam = 0.0f;

	lCam = 0.0f;
	vCam = 0.0f;
	dCam = 0.0f;

	//Set initial light
	aLit = 0.0f;
	eLit = 0.75f;

	// Initialise timer
	gettimeofday(&tv, NULL);
	t0 = (double)tv.tv_sec + tv.tv_usec / 1000000.0f;
	t1 = t0;
	t2 = t0;

	return 0;
}

/*
 * render
 */

void render(void)
{
static GLint iFrames = 0;
static GLfloat fps = 0.0f, DeltaT;
static char cBuffer[64];
struct timeval tv;
GLuint model_id;

	// Update timer
	gettimeofday(&tv, NULL);
	etime = (double)tv.tv_sec + tv.tv_usec / 1000000.0f;

	dt = etime - t0;
	t0 = etime;

	/*
	 * Make the shadow pass
	 *
	 */

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, FBO );

	glPushMatrix();

		//Compute light position
		sinE = sinf(eLit);
		cosE = cosf(eLit);
		sinA = sinf(aLit);
		cosA = cosf(aLit);

		lightPos[0] = lightRadius * cosE * sinA;
		lightPos[1] = lightRadius * sinE;
		lightPos[2] = lightRadius * cosE * cosA;
		lightPos[3] = 1.0f;

		//Set light position
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		//Set up camera to light location
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( 70.0f, 1.0f, 75.0f, 350.0f );
		glGetDoublev(GL_PROJECTION_MATRIX, lightProjection );
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( lightPos[0], lightPos[1], lightPos[2],
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
		glGetDoublev(GL_MODELVIEW_MATRIX, lightModelview );
		glViewport( 0, 0, shadow_sz, shadow_sz );

		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glShadeModel(GL_FLAT);

		//Rotate scene, if required
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);

		//Disable shaders
		glUseProgramObjectARB(0);
		glDisable(GL_VERTEX_PROGRAM_ARB);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		// Draw dynamic objects
		for (model_id = 0; model_id < NUM_MODELS; model_id++ )
		{
			//Update the kinematic's state
			UpdateMD2(md2_model[model_id], dt);
	
			//Animate the MD2 models
			AnimateMD2(md2_model[model_id], dt);
			
			//Draw the geometry
			glPushMatrix();
				glTranslatef(	md2_model[model_id]->position.x,
						md2_model[model_id]->position.y,
						md2_model[model_id]->position.z );
				glRotatef( md2_model[model_id]->rotation, 0.0f, 1.0f, 0.0f );
				DrawMD2(md2_model[model_id]);
			glPopMatrix();
		}

	glPopMatrix();

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

	/*
	 * And now the normal pass
	 *
	 */

	//Back to normal settings
	glDisable(GL_POLYGON_OFFSET_FILL);
	glShadeModel(GL_SMOOTH);
	change_size( width, height );

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

		//Set up camera location and parameters
		setup_camera((float)dt);

		//Retrieve modelview matrix and invert it
		glGetDoublev(GL_MODELVIEW_MATRIX, cameraModelview );
		FastInvert4( cameraModelview, cameraModelviewInverse );

		//Set up depth texture
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadow_tx);

		//Set up texture matrix for shadow map projection
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(0.5f, 0.5f, 0.5f);
		glScalef(0.5f, 0.5f, 0.5f);
		glMultMatrixd(lightProjection);
		glMultMatrixd(lightModelview);
		glMultMatrixd(cameraModelviewInverse);
		glMatrixMode(GL_MODELVIEW);

		//Rotate scene
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);

		//Re-enable shaders
		glEnable(GL_VERTEX_PROGRAM_ARB);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		if (GLSLshader)
			glUseProgramObjectARB(progObj);
		else
			glUseProgramObjectARB(0);

		//Floor

		//Color
		glColor3f(0.64f, 0.63f, 0.65f);

		//Set textures
		glActiveTexture(GL_TEXTURE0);	//Diffuse map
		glBindTexture(GL_TEXTURE_2D, texture_id[NUM_TEXTURES-2]);

		glActiveTexture(GL_TEXTURE1);	//Normal map
		glBindTexture(GL_TEXTURE_2D, texture_id[NUM_TEXTURES-1]);

		//Set Tangent vector
		glVertexAttrib3fv( tangent, floorT );

		//Set Binormal vector
		glVertexAttrib3fv( binormal, floorB );

		//Set Normal vector
		glNormal3fv( floorN );

		//Call Display List to draw
		glCallList(FList);

		// Draw dynamic objects
		for (model_id = 0; model_id < NUM_MODELS; model_id++ )
		{
			//Set color
			glColor3f(	md2_model[model_id]->color.x,
					md2_model[model_id]->color.y,
					md2_model[model_id]->color.z );
	
			//Set texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id[2*model_id]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture_id[2*model_id+1]);
	
			//Draw the geometry
			glPushMatrix();
				glTranslatef(	md2_model[model_id]->position.x,
						md2_model[model_id]->position.y,
						md2_model[model_id]->position.z );
				glRotatef( md2_model[model_id]->rotation, 0.0f, 1.0f, 0.0f );
				DrawMD2(md2_model[model_id]);
			glPopMatrix();
		}

	glPopMatrix();

	iFrames++;
	DeltaT = (GLfloat)(etime-t1);
	if( DeltaT >= Timed )
	{
		fps = (GLfloat)(iFrames)/DeltaT;
		fps_count++;
		fps_mean = ((fps_count - 1.0f) * fps_mean + fps ) / fps_count;

		iFrames = 0;
		t1 = etime;

		sprintf(cBuffer,"FPS: %.1f Mean FPS: %.1f Poly Scale: %.1f Bias: %.1f", fps, fps_mean, scalePoly, biasPoly);
	}
	if (print_fps)
	{
		if (windowpos)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			glPushAttrib(GL_LIST_BIT);
				glListBase(fps_font - ' ');
				glWindowPos2i(0,2);
				glCallLists(strlen(cBuffer), GL_UNSIGNED_BYTE, cBuffer);
			glPopAttrib();
		}
		if( iFrames == 0 )
			printf("FPS: %.1f Mean FPS: %.1f\n", fps, fps_mean);
	}
	lCam = 0.0f;
	vCam = 0.0f;
	dCam = 0.0f;
}

/*
 * handle_mouse_button
 */

void handle_mouse_button( int button, int state, int x, int y )
{
	if (state)
		return;
	switch (button)
	{
	case 1:
		old_x = x;
		old_y = y;
		break;
	case 3:
		old_x = x;
		old_y = y;
		break;
	case 4:
		dCam = -tStep;
		break;
	case 5:
		dCam = tStep;
	}
}

/*
 * handle_mouse_motion
 */

void handle_mouse_motion(int button, int x, int y )
{
	switch (button)
	{
	case 1:
		eCam += (y-old_y) * mouse_scale_t;
		aCam += (x-old_x) * mouse_scale_t;
		old_y = y;
		old_x = x;
		break;
	case 3:
		xRot -=(y-old_y) * mouse_scale_r;
		yRot -=(x-old_x) * mouse_scale_r;
		old_y = y;
		old_x = x;
	}
}

/*
 * event_loop
 */

void event_loop( Display* display, Window window )
{
while (1)
	{
	while (XPending(display) > 0)
		{
	        	XEvent event;
			XNextEvent( display, &event );

			switch (event.type)
			{
				case Expose:
					break;
				case ConfigureNotify:
					change_size(event.xconfigure.width, event.xconfigure.height);
					break;
				case KeyPress:
				{
					int code;

					code = XLookupKeysym( &event.xkey, 0);

					if ((code == XK_Escape)||(code == XK_q))
						return;
					if (code == XK_Up)
					{
						eLit += lStep;
						if ( eLit > PI)
							eLit = PI;
					}
					if (code == XK_Down)
					{
						eLit -= lStep;
						if (eLit < 0.f)
							eLit = 0.f;
					}
					if (code == XK_Left)
						aLit -= lStep;
					if (code == XK_Right)
						aLit += lStep;
					if(code == XK_w)
						dCam = tStep;
					if(code == XK_s)
						dCam = -tStep;
					if(code == XK_d)
						lCam = tStep;
					if(code == XK_a)
						lCam = -tStep;
					if(code == XK_g)
						GLSLshader = !GLSLshader;
					if (code == XK_f)
						print_fps = !print_fps;
					if (code == XK_o)
					{
				        	scalePoly++;
        					glPolygonOffset(scalePoly, biasPoly);
					}
					if (code == XK_i)
					{
						scalePoly--;
						glPolygonOffset(scalePoly, biasPoly);
					}
					if (code == XK_l)
					{
				        	biasPoly++;
        					glPolygonOffset(scalePoly, biasPoly);
					}
					if (code == XK_k)
					{
						biasPoly--;
						glPolygonOffset(scalePoly, biasPoly);
					}
					if (code == XK_p) // toggle character 0 running/standing
					{
						md2_model[0]->anim_command = ANIMATION_CHANGE;
						md2_model[1]->anim_command = ANIMATION_CHANGE;
						if (md2_model[0]->current_animation == ANIMATION_RUN)
						{
							md2_model[0]->anim_state = ANIMATION_STANDING_IDLE;
							md2_model[1]->anim_state = ANIMATION_STANDING_IDLE;
						}
						else
						{
							md2_model[0]->anim_state = ANIMATION_RUN;
							md2_model[1]->anim_state = ANIMATION_RUN;
						}
					}
					if (code == XK_0) // increase character 1 animation
					{
						md2_model[2]->anim_command = ANIMATION_CHANGE;
						if ( md2_model[2]->anim_state++ == ANIMATION_CROUCH_DEATH )
							md2_model[2]->anim_state = ANIMATION_STANDING_IDLE;
					}
					if (code == XK_9) // decrease character 1 animation
					{
						md2_model[2]->anim_command = ANIMATION_CHANGE;
						if ( md2_model[2]->anim_state-- == ANIMATION_STANDING_IDLE )
							md2_model[2]->anim_state = ANIMATION_CROUCH_DEATH;
					}
					if (code == XK_h) // kill character 1
					{
						md2_model[2]->anim_command = ANIMATION_CHANGE;
						md2_model[2]->anim_state = ANIMATION_DEATH_SLOW;
					}
					if (code == XK_r)
					{
						wireframe = !wireframe;
						if (wireframe)
							glPolygonMode(GL_FRONT, GL_LINE);
						else
						glPolygonMode(GL_FRONT, GL_FILL);
					}
				}
				case ButtonPress:
				{
					handle_mouse_button(event.xbutton.button, 0, event.xbutton.x, event.xbutton.y);
					break;
				}
				case ButtonRelease:
				{
					handle_mouse_button(event.xbutton.button, 1, event.xbutton.x, event.xbutton.y);
					break;
				}
				case MotionNotify:
				{
					if (event.xmotion.state & Button1Mask)
					{
						handle_mouse_motion(1, event.xmotion.x, event.xmotion.y);
						break;
					}
					if (event.xmotion.state & Button3Mask)
						handle_mouse_motion(3, event.xmotion.x, event.xmotion.y);
					break;
				}
			}
		}

	render();
	glXSwapBuffers( display, window );

	switch (signal_pending)
	{
		case SIGINT:
		case SIGHUP:
		case SIGTERM:
			return;	/* return from event_loop */
		default:
			break;
	}

	signal_pending = 0;

	if( (use_timer) && ( (etime - t2) > max_timer ) )
		return;

	}
}

/*
 * main()
 */

int main(int argc, char *argv[])
{
char*			command_name;
char*			display_name;
Display*		display;
Window			window;
int			scrnum;
Window			root;
int			x, y;
GLXContext		context;
XVisualInfo*		vinfo;
XSetWindowAttributes	winattrs;
XFontStruct*		font;
unsigned long		winmask;
static const char* getopt_string = "sf::ima::r:vh";
static const struct option longopts[] =
{
	{ "full", no_argument, NULL, 's' },
	{ "fps", optional_argument, NULL, 'f' },
	{ "info", no_argument, NULL, 'i' },
	{ "fsaa", no_argument, NULL, 'm' },
	{ "aniso", optional_argument, NULL, 'a' },
	{ "runlength", required_argument, NULL, 'r' },
	{ "version", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, 'h' },
	{ 0, 0, 0, 0 }
};
static const char* x11font = "-*-courier-bold-r-normal--14-*-*-*-*-*-*-*";
static int		attributes[] = {
					GLX_RGBA,
					GLX_DOUBLEBUFFER,
					GLX_RED_SIZE, 8,
					GLX_GREEN_SIZE, 8,
					GLX_BLUE_SIZE, 8,
					GLX_ALPHA_SIZE, 8,
					GLX_DEPTH_SIZE, 24,
					0
					};

/* Comman line arguments processing */

command_name = basename(argv[0]);

while (1)
{
	int c = getopt_long(argc, argv, getopt_string, longopts, NULL);

	if (c == -1)
		break;

	switch (c)
	{
	case 's':
		full_screen = GL_TRUE;
		break;
	case 'f':
		if( optarg == NULL )
		{
			print_fps = GL_TRUE;
		}
		else
			if( ( Timed = (GLfloat)atof(optarg) ) > 0.0f )
				print_fps = GL_TRUE;
			else
			{
				printf("<!> Time slice for fps computation must be greater than 0\n");
				return 0;
			}

		break;
	case 'i':
		print_info = GL_TRUE;
		break;
	case 'm':
		enable_fsaa = GL_TRUE;
		break;
	case 'a':
		if( optarg == NULL )
		{
			enable_anisotropic = GL_TRUE;
			AnisoLevel = 16.0f;
		}
		else
			if( ( AnisoLevel = (GLfloat)atof(optarg) ) > 0.0f )
				if( 	( AnisoLevel ==  2.0f ) ||
					( AnisoLevel ==  4.0f ) ||
					( AnisoLevel ==  8.0f ) ||
					( AnisoLevel == 16.0f ) )
						enable_anisotropic = GL_TRUE;
				else
				{
					printf("<!> Incorrect anisotropic filtering level specified, must be 2, 4, 8 or 16\n");
					return 0;
				}
			else
			{
				printf("<!> Incorrect anisotropic filtering level specified, must be 2, 4, 8 or 16\n");
				return 0;
			}

		break;
	case 'r':
		if( ( max_timer = (GLfloat)atof(optarg) ) > 0.0f )
			use_timer = GL_TRUE;
		else
		{
			printf("<!> Timer must be greater than 0\n");
			return 0;
		}

		break;
	case 'v':
		printf("Version: " VERSION "\n");
		return 0;
	case 'h':
		printf("Usage: %s [OPTIONS]...\n"
				"%s is an OpenGL rendering demo.\n"
				"   -s, --full         : display full screen\n"
				"   -f[t], --fps[=t]   : display frames per second every t seconds (default 5)\n"
				"   -i, --info         : display X11/OpenGL and program info\n"
				"   -m, --fsaa         : enable anti-aliasing\n"
				"   -a[l], --aniso[=l] : enable xl anisotropic filtering (default 16)\n"
				"   -rL, --runlength=L : run the demo for L secs, then exit\n"
				"   -v, --version      : display version and exit\n",
				"   -h, --help         : display this information\n",
				command_name, command_name
		);
		return 0;
	case '?':
	case ':':
		exit(-1);
	}
}

/* Set signal handlers */

act.sa_handler = signal_handler;
sigemptyset(&act.sa_mask);
act.sa_flags = 0;

if( (sigaction(SIGINT, &act, &oact) < 0)
	|| (sigaction(SIGTERM, &act, &oact) < 0)
	|| (sigaction(SIGHUP, &act, &oact) < 0) )
{
	printf("<!> Setting signal handler failed\n");
	return 1;
}

/* Open the connection to the X server */

display_name = getenv("DISPLAY");
display = XOpenDisplay(display_name);

if (!display)
{
	printf("<!> Couldn't open display %s\n", display_name);
	return 1;
}
else if (print_info)
	printf("<-> Name of display: %s\n", DisplayString(display));

/* Find the proper visual */

scrnum = DefaultScreen( display );
root = RootWindow( display, scrnum );

x = 0; y = 0;

if (full_screen)
{
	width = DisplayWidth( display, scrnum );
	height = DisplayHeight( display, scrnum );
}
else
{
	width = DisplayWidth( display, scrnum )*3/4;
	height = DisplayHeight( display, scrnum )*3/4;
}

vinfo = glXChooseVisual(display, scrnum, attributes);

if (!vinfo)
{
	printf("<!> Couldn't get an RGB, Double-buffered visual\n");
	return 1;
}
else if (print_info)
	printf("<-> Visual ID: %x\n", vinfo->visualid);

/* Create the window */

winattrs.event_mask =	ExposureMask | StructureNotifyMask |
			ButtonPressMask | ButtonReleaseMask |
			PointerMotionMask | KeyPressMask;

winattrs.background_pixel = BlackPixel(display, scrnum);
winattrs.border_pixel = BlackPixel(display, scrnum);
winattrs.bit_gravity = StaticGravity;
winattrs.colormap = XCreateColormap( display, root, vinfo->visual, AllocNone);
winattrs.override_redirect = full_screen;

winmask = CWBitGravity | CWBackPixel | CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect;

window = XCreateWindow( display, root,
			x, y, width, height, 0, vinfo->depth, InputOutput,
			vinfo->visual, winmask, &winattrs );

XChangeProperty(display, window, XA_WM_NAME, XA_STRING, 8, 0,
		command_name, strlen(argv[0]));
XChangeProperty(display, window, XA_WM_ICON_NAME, XA_STRING, 8, 0,
		command_name, strlen(argv[0]));

/* Create the OpenGL context */

context = glXCreateContext(display, vinfo, 0, True);

if (!context)
{
	printf("<!> glXCreateContext failed\n");
	return 1;
}

XFree(vinfo);

XMapWindow( display, window );
glXMakeCurrent( display, window, context );

/* Print info */

if (print_info)
{
	printf("<-> GL_RENDERER   = %s\n", (char *) glGetString(GL_RENDERER));
	printf("<-> GL_VERSION    = %s\n", (char *) glGetString(GL_VERSION));
	printf("<-> GL_VENDOR     = %s\n", (char *) glGetString(GL_VENDOR));
	printf("<-> GL_EXTENSIONS = %s\n", (char *) glGetString(GL_EXTENSIONS));
}

/* Setup fonts */
if( ( font = XLoadQueryFont(display, x11font) ) == NULL )
{
	//Try fallback first
	if( ( font = XLoadQueryFont(display, "fixed") ) == NULL )
	{
		printf("<!> Can't load font \"%s\"\n", x11font);
		return -1;
	}
	else if (print_info)
		printf("<-> Loaded font \"fixed\"\n");
}
else if (print_info)
	printf("<-> Loaded font \"%s\"\n", x11font);

fps_font = glGenLists(96);
glXUseXFont(font->fid, ' ', 96, fps_font);


/* Setup Rendering Context */

if (setup())
{
	printf("<!> Setup failed, exiting ...\n");
	return 1;
}

/* Event loop */

event_loop( display, window );

/* Print average fps */
printf("Average FPS: %.1f\n", fps_mean);

/* Clean up */

// Delete shader identifiers
glDeleteProgramsARB(NUM_SHADERS, ids);

// Delete shader objects
glDeleteObjectARB(progObj);

//Texture objects
glDeleteTextures( NUM_TEXTURES, texture_id );
if (texture_id)
	free(texture_id);

//Lists
glDeleteLists(fps_font, 96);
glDeleteLists(FList, 1);

//MD2 models
int model_id;

for (model_id = 0; model_id < NUM_MODELS; ++model_id)
	if (md2_model[model_id])
	{
		if (md2_model[model_id]->normal)
		{
			free(md2_model[model_id]->normal);
			md2_model[model_id]->normal = NULL;
		}
		if (md2_model[model_id]->skin)
		{
			free(md2_model[model_id]->skin);
			md2_model[model_id]->skin = NULL;
		}
		free(md2_model[model_id]);
		md2_model[model_id] = NULL;
	}
	
//Floor
if (floor_texture)
{
	free(floor_texture);
	floor_texture = NULL;
}

//Destroy FB objects
glDeleteTextures(1, &shadow_tx);
glDeleteFramebuffersEXT(1, &FBO);

glXDestroyContext( display, context );
XDestroyWindow( display, window );
XCloseDisplay( display );

if( print_info )
	printf("Program exit successfully\n");

return 0;
}
