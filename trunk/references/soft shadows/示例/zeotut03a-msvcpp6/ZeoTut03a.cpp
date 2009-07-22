/************************************
*                                   *
*    Overlords Tutorial - 02b       *
*     http://www.flashbang.se       *
*             2006                  *
*                                   *
************************************/

#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glext.h>											// Header File For The GLaux Library
#include "ZeoBase.h"
#include <fstream.h>											
#include "cv90.h"
#include "tex.h"


#pragma comment( lib, "opengl32.lib" )							// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )								// Search For GLu32.lib While Linking

#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

GL_Window*	g_window;
Keys*		g_keys;

// User Defined Variables
float		angle;												// Used To Rotate The Triangles
float lpos[3];


//other fbo stuff
unsigned int fb;
unsigned int color_rb;
unsigned int depth_tex;


float lightProjectionMatrix[16], lightViewMatrix[16];
float cameraProjectionMatrix[16], cameraViewMatrix[16], textureMatrix[16];



// multitexturing
static PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB;
static PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB;
static PFNGLMULTITEXCOORD3FARBPROC	glMultiTexCoord3fARB;
static PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB;
static PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB;
static PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;


// shader stuff
static PFNGLDELETEOBJECTARBPROC                    glDeleteObjectARB;
static PFNGLGETHANDLEARBPROC                       glGetHandleARB;
static PFNGLDETACHOBJECTARBPROC                    glDetachObjectARB;
static PFNGLCREATESHADEROBJECTARBPROC              glCreateShaderObjectARB;
static PFNGLSHADERSOURCEARBPROC                    glShaderSourceARB;
static PFNGLCOMPILESHADERARBPROC                   glCompileShaderARB;
static PFNGLCREATEPROGRAMOBJECTARBPROC             glCreateProgramObjectARB;
static PFNGLATTACHOBJECTARBPROC                    glAttachObjectARB;
static PFNGLLINKPROGRAMARBPROC                     glLinkProgramARB;
static PFNGLUSEPROGRAMOBJECTARBPROC                glUseProgramObjectARB;
static PFNGLVALIDATEPROGRAMARBPROC                 glValidateProgramARB;

static PFNGLUNIFORM1FARBPROC                       glUniform1fARB;
static PFNGLUNIFORM2FARBPROC                       glUniform2fARB;
static PFNGLUNIFORM3FARBPROC                       glUniform3fARB;
static PFNGLUNIFORM4FARBPROC                       glUniform4fARB;
static PFNGLUNIFORM1IARBPROC                       glUniform1iARB;
static PFNGLUNIFORM2IARBPROC                       glUniform2iARB;
static PFNGLUNIFORM3IARBPROC                       glUniform3iARB;
static PFNGLUNIFORM4IARBPROC                       glUniform4iARB;
static PFNGLUNIFORM1FVARBPROC                      glUniform1fvARB;
static PFNGLUNIFORM2FVARBPROC                      glUniform2fvARB;
static PFNGLUNIFORM3FVARBPROC                      glUniform3fvARB;
static PFNGLUNIFORM4FVARBPROC                      glUniform4fvARB;
static PFNGLUNIFORM1IVARBPROC                      glUniform1ivARB;
static PFNGLUNIFORM2IVARBPROC                      glUniform2ivARB;
static PFNGLUNIFORM3IVARBPROC                      glUniform3ivARB;
static PFNGLUNIFORM4IVARBPROC                      glUniform4ivARB;
static PFNGLUNIFORMMATRIX2FVARBPROC                glUniformMatrix2fvARB;
static PFNGLUNIFORMMATRIX3FVARBPROC                glUniformMatrix3fvARB;
static PFNGLUNIFORMMATRIX4FVARBPROC                glUniformMatrix4fvARB;

static PFNGLGETOBJECTPARAMETERFVARBPROC            glGetObjectParameterfvARB;
static PFNGLGETOBJECTPARAMETERIVARBPROC            glGetObjectParameterivARB;
static PFNGLGETINFOLOGARBPROC                      glGetInfoLogARB;
static PFNGLGETATTACHEDOBJECTSARBPROC              glGetAttachedObjectsARB;
static PFNGLGETUNIFORMLOCATIONARBPROC              glGetUniformLocationARB;
static PFNGLGETACTIVEUNIFORMARBPROC                glGetActiveUniformARB;
static PFNGLGETUNIFORMFVARBPROC                    glGetUniformfvARB;
static PFNGLGETUNIFORMIVARBPROC                    glGetUniformivARB;
static PFNGLGETSHADERSOURCEARBPROC                 glGetShaderSourceARB;

static PFNGLISRENDERBUFFEREXTPROC    glIsRenderbufferEXT    = NULL;
static PFNGLBINDRENDERBUFFEREXTPROC    glBindRenderbufferEXT     = NULL;
static PFNGLDELETERENDERBUFFERSEXTPROC    glDeleteRenderbuffersEXT    = NULL;
static PFNGLGENRENDERBUFFERSEXTPROC    glGenRenderbuffersEXT    = NULL;
static PFNGLRENDERBUFFERSTORAGEEXTPROC    glRenderbufferStorageEXT    = NULL;
static PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC    glGetRenderbufferParameterivEXT    = NULL;
static PFNGLISFRAMEBUFFEREXTPROC    glIsFramebufferEXT    = NULL;
static PFNGLBINDFRAMEBUFFEREXTPROC   glBindFramebufferEXT    = NULL;
static PFNGLDELETEFRAMEBUFFERSEXTPROC    glDeleteFramebuffersEXT    = NULL;
static PFNGLGENFRAMEBUFFERSEXTPROC    glGenFramebuffersEXT    = NULL;
static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC    glCheckFramebufferStatusEXT    = NULL;
static PFNGLFRAMEBUFFERTEXTURE1DEXTPROC    glFramebufferTexture1DEXT    = NULL;
static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2DEXT    = NULL;
static PFNGLFRAMEBUFFERTEXTURE3DEXTPROC    glFramebufferTexture3DEXT    = NULL;
static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC    glFramebufferRenderbufferEXT    = NULL;
static PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC    glGetFramebufferAttachmentParameterivEXT    = NULL;
static PFNGLGENERATEMIPMAPEXTPROC   glGenerateMipmapEXT     = NULL;

GLhandleARB  ProgramObject;
GLhandleARB  VertexShaderObject;
GLhandleARB  FragmentShaderObject;
char*     VertexShaderSource;
char*     FragmentShaderSource;
unsigned int useshader;

unsigned int tex1,tex2,tex3,tex4;

void ViewOrtho(int x, int y)							// Set Up An Ortho View
{
	glMatrixMode(GL_PROJECTION);					// Select Projection
	glPushMatrix();							// Push The Matrix
	glLoadIdentity();						// Reset The Matrix
	glOrtho( 0, x , y , 0, -1, 1 );				// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);					// Select Modelview Matrix
	glPushMatrix();							// Push The Matrix
	glLoadIdentity();						// Reset The Matrix
}

void ViewPerspective(void)							// Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );					// Select Projection
	glPopMatrix();							// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );					// Select Modelview
	glPopMatrix();							// Pop The Matrix
}

// checks if a extentionn is supported, we need to do this since not all harware or drivers support this
bool IsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

void checkfbo(void)
{
	/* lots of booring errorchecking stuff, this will display an error code and then kill the app if it failed*/
	switch(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {                                          
          case GL_FRAMEBUFFER_COMPLETE_EXT: 
			  	return;
			break; 


			 
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				 TerminateApplication (g_window); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				TerminateApplication (g_window); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				 TerminateApplication (g_window); 
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				TerminateApplication (g_window); 
				 break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				TerminateApplication (g_window); 
				 break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				TerminateApplication (g_window); 
				 break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
				TerminateApplication (g_window); 
				 break;


			case GL_FRAMEBUFFER_UNSUPPORTED_EXT: 
					MessageBox (HWND_DESKTOP, "GL_FRAMEBUFFER_UNSUPPORTED_EXT", "Error", MB_OK | MB_ICONEXCLAMATION);
					TerminateApplication (g_window); 
		
		     break;   
                                              
          default:                                               
            TerminateApplication (g_window);                                        
	}
}


unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}

void loadVShade(char filename[160])  //loads a vertex shader in glsl format
{

	ifstream file;
    file.open(filename, ios::in);
    if(!file) {useshader=0; return;}
    
    unsigned long len = getFileLength(file);
    
    if (len==0) {useshader=0; return;}   // "Empty File" 
    
    
    VertexShaderSource = new char[len+1];
    if (VertexShaderSource == 0) {useshader=0;  return;}   // can't reserve memory
    
    VertexShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
                            // it is important to 0-terminate the real length later, len is just max possible value...
   
    unsigned int i=0;
    while (file.good())
    {
        VertexShaderSource[i++] = file.get();       // get character from file
        if (i>len) i=len;   // coding guidelines...
    }
    i--;
    VertexShaderSource[i] = 0;  // 0 terminate it.
    
    file.close();   

	return;
}




void loadFShade(char filename[160])  //dito on a fragment shader
{


	ifstream file;
    file.open(filename, ios::in);
    if(!file) {useshader=0; return;}
    
    unsigned long len = getFileLength(file);
    
    if (len==0) {useshader=0; return;}   // "Empty File" 
    
    
    FragmentShaderSource =  new char[len+1];
    if (FragmentShaderSource == 0) {useshader=0; return;}   // can't reserve memory
    
    FragmentShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
                            // it is important to 0-terminate the real length later, len is just max possible value...
   
    unsigned int i=0;
    while (file.good())
    {
        FragmentShaderSource[i++] = file.get();       // get character from file
        if (i>len) i=len;   // coding guidelines...
    }
    
	i--;
    FragmentShaderSource[i] = 0;  // 0 terminate it.
    
    file.close();   

	return;
}


void compileShaders(void)
{

	int compiled = 0;
	int linked   = 0;
	char str[4096];

	useshader=1;

	ProgramObject	   = glCreateProgramObjectARB();
	VertexShaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	FragmentShaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	

	glShaderSourceARB(VertexShaderObject, 1, (const char **)&VertexShaderSource, NULL);
	glShaderSourceARB(FragmentShaderObject, 1, (const char **)&FragmentShaderSource, NULL);

	delete[] VertexShaderSource;
	delete[] FragmentShaderSource;

	// Compile the vertex and fragment shader, and print out the
	glCompileShaderARB(VertexShaderObject);
	glGetObjectParameterivARB(VertexShaderObject,GL_OBJECT_COMPILE_STATUS_ARB, &compiled);

	if (!compiled) {

		glGetInfoLogARB( VertexShaderObject, sizeof(str), NULL, str );
		MessageBox( NULL, str, "vertex Shader Compile Error", MB_OK|MB_ICONEXCLAMATION );
	    useshader=0;
	    return;
	}

	glCompileShaderARB(FragmentShaderObject);
	glGetObjectParameterivARB(FragmentShaderObject,GL_OBJECT_COMPILE_STATUS_ARB, &compiled);

	if (!compiled) {
		glGetInfoLogARB( FragmentShaderObject, sizeof(str), NULL, str );
		MessageBox( NULL, str, "Fragment Shader Compile Error", MB_OK|MB_ICONEXCLAMATION );
	    useshader=0;
	    return;
	}

	glAttachObjectARB(ProgramObject,VertexShaderObject);
	glAttachObjectARB(ProgramObject,FragmentShaderObject);

	glDeleteObjectARB(VertexShaderObject);
	glDeleteObjectARB(FragmentShaderObject);

	glLinkProgramARB(ProgramObject);
	glGetObjectParameterivARB(ProgramObject, GL_OBJECT_LINK_STATUS_ARB,	&linked);

	if (!linked) {
		MessageBox (HWND_DESKTOP, "can't link shaders", "Error", MB_OK | MB_ICONEXCLAMATION);
	    useshader=0;
	    return;
	}

    return;
}

// uniform funcs, you need these to pass on constants to the shader, these are pretty self explanatory

void sendUniform1i(char var[60], int v)
{
	GLint loc = glGetUniformLocationARB(ProgramObject, var);
    if (loc==-1) return;  // can't find variable
    
    glUniform1iARB(loc, v);
}

void sendUniform1f(char var[60], float v)
{
	GLint loc = glGetUniformLocationARB(ProgramObject, var);
    if (loc==-1) return;  // can't find variable
    
    glUniform1fARB(loc, v);
}

void sendUniform2f(char var[60], float v[2])
{
	GLint loc = glGetUniformLocationARB(ProgramObject, var);
    if (loc==-1) return;  // can't find variable
    
    glUniform2fARB(loc, v[0], v[1]);
}

void sendUniform3f(char var[60], float v[3])
{
	GLint loc = glGetUniformLocationARB(ProgramObject, var);
    if (loc==-1) return;  // can't find variable
    
    glUniform3fARB(loc, v[0], v[1], v[2]);
}

void sendUniform4f(char var[60], float v[4])
{
	GLint loc = glGetUniformLocationARB(ProgramObject, var);
    if (loc==-1) return;  // can't find variable
    
    glUniform4fARB(loc, v[0], v[1], v[2], v[03]);
}


void Combine_Matrix4(float MatrixA[16],float  MatrixB[16], float *retM)
{
   float  NewMatrix[16];
   int i;

   for(i = 0; i < 4; i++){  //Cycle through each vector of first matrix.
	NewMatrix[i*4]   = MatrixA[i*4] * MatrixB[0] + MatrixA[i*4+1] * MatrixB[4] + MatrixA[i*4+2] * MatrixB[8] + MatrixA[i*4+3] * MatrixB[12];
	NewMatrix[i*4+1] = MatrixA[i*4] * MatrixB[1] + MatrixA[i*4+1] * MatrixB[5] + MatrixA[i*4+2] * MatrixB[9] + MatrixA[i*4+3] * MatrixB[13];
 	NewMatrix[i*4+2] = MatrixA[i*4] * MatrixB[2] + MatrixA[i*4+1] * MatrixB[6] + MatrixA[i*4+2] * MatrixB[10] + MatrixA[i*4+3] * MatrixB[14];	
 	NewMatrix[i*4+3] = MatrixA[i*4] * MatrixB[3] + MatrixA[i*4+1] * MatrixB[7] + MatrixA[i*4+2] * MatrixB[11] + MatrixA[i*4+3] * MatrixB[15];
     }
   /*this should combine the matrixes*/

   memcpy(retM,NewMatrix,64);
}

BOOL Initialize (GL_Window* window, Keys* keys)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;



	// Start Of User Initialization
	angle		= 0.0f;											// Set Starting Angle To Zero

	lpos[0]=30;
	lpos[1]=40;
	lpos[2]=30;

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);						// Black Background
	glClearDepth (1.0f);										// Depth Buffer Setup
	glDepthFunc (GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable (GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel (GL_SMOOTH);									// Select Smooth Shading
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
	


		// create the needed ext bundings for fbos
	glIsRenderbufferEXT    = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
	glBindRenderbufferEXT    = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
	glDeleteRenderbuffersEXT    = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
	glGenRenderbuffersEXT    = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
	glRenderbufferStorageEXT    = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
	glGetRenderbufferParameterivEXT    = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
	glIsFramebufferEXT    = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
	glBindFramebufferEXT    = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
	glDeleteFramebuffersEXT    = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
	glGenFramebuffersEXT    = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
	glCheckFramebufferStatusEXT    = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	glFramebufferTexture1DEXT    = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
	glFramebufferTexture2DEXT    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glFramebufferTexture3DEXT    = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
	glFramebufferRenderbufferEXT    = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
	glGetFramebufferAttachmentParameterivEXT    = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
	glGenerateMipmapEXT     = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");


	glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
	glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
	glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
	glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");	

	// check if GL_ARB_shading_language_100 can be use on this system
	if (!IsExtensionSupported("GL_ARB_shading_language_100"))
	{
		MessageBox (HWND_DESKTOP, "Sorry, but you can't use shaders, try upgrading you drivers or hardware", "Error", MB_OK | MB_ICONEXCLAMATION);
		TerminateApplication (g_window);
	}

	else
	{

	//GLSLang extentions, handle with care.
	glDeleteObjectARB			= (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
	glGetHandleARB				= (PFNGLGETHANDLEARBPROC) wglGetProcAddress("glGetHandleARB");
	glDetachObjectARB			= (PFNGLDETACHOBJECTARBPROC) wglGetProcAddress("glDetachObjectARB");
	glCreateShaderObjectARB		= (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB			= (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB			= (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB	= (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB			= (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB			= (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB		= (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
	glValidateProgramARB		= (PFNGLVALIDATEPROGRAMARBPROC) wglGetProcAddress("glValidateProgramARB");
	glGetObjectParameterfvARB	= (PFNGLGETOBJECTPARAMETERFVARBPROC) wglGetProcAddress("glGetObjectParameterfvARB");
	glGetObjectParameterivARB	= (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
	glGetInfoLogARB				= (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
	glGetAttachedObjectsARB		= (PFNGLGETATTACHEDOBJECTSARBPROC) wglGetProcAddress("glGetAttachedObjectsARB");
	glGetUniformLocationARB		= (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
	glGetActiveUniformARB		= (PFNGLGETACTIVEUNIFORMARBPROC) wglGetProcAddress("glGetActiveUniformARB");
	glGetUniformfvARB			= (PFNGLGETUNIFORMFVARBPROC) wglGetProcAddress("glGetUniformfvARB");
	glGetUniformivARB			= (PFNGLGETUNIFORMIVARBPROC) wglGetProcAddress("glGetUniformivARB");
	glGetShaderSourceARB		= (PFNGLGETSHADERSOURCEARBPROC) wglGetProcAddress("glGetShaderSourceARB");
                                                                     
	glUniform1fARB= (PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
	glUniform2fARB= (PFNGLUNIFORM2FARBPROC) wglGetProcAddress("glUniform2fARB");
	glUniform3fARB= (PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB");
	glUniform4fARB= (PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniform1iARB= (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
	glUniform2iARB= (PFNGLUNIFORM2IARBPROC) wglGetProcAddress("glUniform2iARB");
	glUniform3iARB= (PFNGLUNIFORM3IARBPROC) wglGetProcAddress("glUniform3iARB");
	glUniform4iARB= (PFNGLUNIFORM4IARBPROC) wglGetProcAddress("glUniform4iARB");
	glUniform1fvARB= (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
	glUniform2fvARB= (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB");
	glUniform3fvARB= (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB");
	glUniform4fvARB= (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB");
	glUniform1ivARB= (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB");
	glUniform2ivARB= (PFNGLUNIFORM2IVARBPROC) wglGetProcAddress("glUniform2ivARB");
	glUniform3ivARB= (PFNGLUNIFORM3IVARBPROC) wglGetProcAddress("glUniform3ivARB");
	glUniform4ivARB= (PFNGLUNIFORM4IVARBPROC) wglGetProcAddress("glUniform4ivARB");
	glUniformMatrix2fvARB= (PFNGLUNIFORMMATRIX2FVARBPROC) wglGetProcAddress("glUniformMatrix2fvARB");
	glUniformMatrix3fvARB= (PFNGLUNIFORMMATRIX3FVARBPROC) wglGetProcAddress("glUniformMatrix3fvARB");
	glUniformMatrix4fvARB= (PFNGLUNIFORMMATRIX4FVARBPROC) wglGetProcAddress("glUniformMatrix4fvARB");


	// init  fbo Nr1

	glGenFramebuffersEXT(1, &fb);													// create a new framebuffer
	glGenTextures(1, &depth_tex);													// and a new texture used as a color buffer
	glGenRenderbuffersEXT(1, &color_rb);											// And finaly a new depthbuffer

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);									// switch to the new framebuffer

	// initialize color texture
	glBindTexture(GL_TEXTURE_2D, depth_tex);										// Bind the colorbuffer texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				// make it linear filterd
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 512, 512, 0,GL_DEPTH_COMPONENT24, GL_INT, NULL);	// Create the texture data
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, depth_tex, 0); // attach it to the framebuffer

	// initialize depth renderbuffer
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, color_rb);							// bind the depth renderbuffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_RGBA,2048, 2048);	// get the data space for it
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_RENDERBUFFER_EXT, color_rb); // bind it to the renderbuffer

	checkfbo();												// make shure it all went ok
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);									// Swithch back to normal framebuffer rendering



	
	useshader=1;

	loadVShade("glsl.vert");									// Load the shader files
	loadFShade("glsl.frag");
	compileShaders();											// compile the shader
	
	tex1=LoadGLTextures("cv90base",2);							// Load the base texture
	tex2=LoadGLTextures("cv90normal",2);					// Load the normal texture

	tex3=LoadGLTextures("groundbase",2);							// Load the base texture
	tex4=LoadGLTextures("groundnormal",2);					// Load the normal texture


	}

	return TRUE;												// Return TRUE (Initialization Successful)
}

void Deinitialize (void)										// Any User DeInitialization Goes Here
{
}

void Update (float seconds)										// Perform Motion Updates Here
{


	//Calculate & save matrices
    glPushMatrix();

    glLoadIdentity();
    gluPerspective(45.0f, (float)800/600, 10.0f, 1000.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

    glLoadIdentity();
    gluLookAt(0, 17, 46,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

    glLoadIdentity();
    gluPerspective(65.0f, 1.0f, 25.0f, 200.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);


    glLoadIdentity();
    gluLookAt( lpos[0], lpos[1], lpos[2],
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);
    glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

    glPopMatrix();


	float tempa[16];
	float biasMatrix[16]={0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f}; //bias from [-1, 1] to [0, 1]

	
	Combine_Matrix4(lightViewMatrix,lightProjectionMatrix, textureMatrix);



	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}

	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}

	angle += seconds * 9.0f;									// Update angle Based On The Clock


}


void RenderPlane(void)
{

	glBegin(GL_QUADS);												// Begin Drawing Quads
	
	glTexCoord2f(0,5);												// Texture  and vertex Coordinates for fullsceen draw
	glVertex3f(-25,0,-25);

	glTexCoord2f(0,0);
	glVertex3f(-25,0,25);

	glTexCoord2f(5,0);
	glVertex3f(25,0,25);

	glTexCoord2f(5,5);
	glVertex3f(25,0,-25);


	glEnd();														// stop drawing quads

}


void Draw (void)
{
	glClearColor (0.0f, 0.5f, 1.0f, 0.5f);
	

	// first pass, set up and render the shadow map

	glViewport (0, 0, 2048, 2048);// set The Current Viewport to the fbo size
	glClearDepth (1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer on the framebuffer to black
	
	glBindTexture(GL_TEXTURE_2D, 0);								// unlink textures because if we dont it all is gonna fail
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);					// switch to rendering on our FBO
	checkfbo();	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear Screen And Depth Buffer on the fbo to red


	glLoadIdentity ();

	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(lightProjectionMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(lightViewMatrix);

	
	glColor3f(1,1,1);

//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(7,0);
	cv90_render();
//	RenderPlane();
//	glDisable(GL_POLYGON_OFFSET_FILL);







	glViewport (0, 0, 800, 600);
	glBindTexture(GL_TEXTURE_2D, 0);								// unlink textures because if we dont it all is gonna fail
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);					// switch to rendering on our FBO

//	checkfbo();													// test if everything is ok
	glClearDepth (1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// second pass render the shadow map

	glLoadIdentity ();


	glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(textureMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(cameraViewMatrix);
	glColor3f(1,1,1);												// set the color to white

	if (useshader) glUseProgramObjectARB(ProgramObject);			// activate the shader

	sendUniform1i("texunit0", 0);									// make shure you set the tex unit uniforms to the correct values
	sendUniform1i("texunit1", 1);
	sendUniform1i("texunit2", 2);

	sendUniform3f("lpos", lpos);									// send the lightposition



	glActiveTextureARB(GL_TEXTURE0_ARB);							// activate and bind texture 1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex1);
	
	glActiveTextureARB(GL_TEXTURE1_ARB);							// ditto with texture 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex2);

	// init the automatic texcoord gemeration
  	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,depth_tex);
	cv90_render();													// render the fabolus tank.


	glActiveTextureARB(GL_TEXTURE0_ARB);							// activate and bind texture 1
	glBindTexture(GL_TEXTURE_2D,tex3);
	
	glActiveTextureARB(GL_TEXTURE1_ARB);							// ditto with texture 2
	glBindTexture(GL_TEXTURE_2D,tex4);


	RenderPlane();

	glActiveTextureARB(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);


	if (useshader) glUseProgramObjectARB(0);						// deactivate the shader

	glFlush ();														// Flush The GL Rendering Pipeline
}
