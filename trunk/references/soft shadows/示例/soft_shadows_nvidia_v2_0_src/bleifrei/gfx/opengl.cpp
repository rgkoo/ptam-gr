
// includes
#include <set>
#include <sstream>

#include "../platform/platform.hpp"
#include "opengl.hpp"

using namespace bleifrei;
using namespace std;

// TEMP
#include "../io/console.hpp"
using namespace bleifrei::io;
// TEMP

set<string> extensions;

// OpenGL_1_2
PFNGLBLENDCOLORPROC glBlendColor;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLCOLORSUBTABLEPROC glColorSubTable;
PFNGLCOLORTABLEPROC glColorTable;
PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
PFNGLGETCOLORTABLEPROC glGetColorTable;
PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
PFNGLGETHISTOGRAMPROC glGetHistogram;
PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
PFNGLGETMINMAXPROC glGetMinmax;
PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
PFNGLHISTOGRAMPROC glHistogram;
PFNGLMINMAXPROC glMinmax;
PFNGLRESETHISTOGRAMPROC glResetHistogram;
PFNGLRESETMINMAXPROC glResetMinmax;
PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;

// OpenGL 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage;

// OpenGL 1.4
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLFOGCOORDPOINTERPROC glFogCoordPointer;
PFNGLFOGCOORDDPROC glFogCoordd;
PFNGLFOGCOORDDVPROC glFogCoorddv;
PFNGLFOGCOORDFPROC glFogCoordf;
PFNGLFOGCOORDFVPROC glFogCoordfv;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
PFNGLPOINTPARAMETERFPROC glPointParameterf;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
PFNGLPOINTPARAMETERIPROC glPointParameteri;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
PFNGLWINDOWPOS2DPROC glWindowPos2d;
PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
PFNGLWINDOWPOS2FPROC glWindowPos2f;
PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
PFNGLWINDOWPOS2IPROC glWindowPos2i;
PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
PFNGLWINDOWPOS2SPROC glWindowPos2s;
PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
PFNGLWINDOWPOS3DPROC glWindowPos3d;
PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
PFNGLWINDOWPOS3FPROC glWindowPos3f;
PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
PFNGLWINDOWPOS3IPROC glWindowPos3i;
PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
PFNGLWINDOWPOS3SPROC glWindowPos3s;
PFNGLWINDOWPOS3SVPROC glWindowPos3sv;

// GL_ARB_vertex_program
PFNGLBINDPROGRAMARBPROC glBindProgramARB;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
PFNGLISPROGRAMARBPROC glIsProgramARB;
PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;

// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

// WGL_ARB_pixel_format
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;

// WGL_ARB_make_current_read
PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB;
PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;

// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;

// WGL_ARB_render_texture
PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;

// GL_EXT_stencil_two_side
PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;

// WGL_EXT_extensions_string
PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;

// GL_ATI_separate_stencil
PFNGLSTENCILOPSEPARATEATIPROC glStencilOpSeparateATI;
PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI;

namespace bleifrei {
	namespace gfx {

		static void init_opengl_1_2(void)
		{
			glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
			glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
			glColorSubTable = (PFNGLCOLORSUBTABLEPROC)wglGetProcAddress("glColorSubTable");
			glColorTable = (PFNGLCOLORTABLEPROC)wglGetProcAddress("glColorTable");
			glColorTableParameterfv = (PFNGLCOLORTABLEPARAMETERFVPROC)wglGetProcAddress("glColorTableParameterfv");
			glColorTableParameteriv = (PFNGLCOLORTABLEPARAMETERIVPROC)wglGetProcAddress("glColorTableParameteriv");
			glConvolutionFilter1D = (PFNGLCONVOLUTIONFILTER1DPROC)wglGetProcAddress("glConvolutionFilter1D");
			glConvolutionFilter2D = (PFNGLCONVOLUTIONFILTER2DPROC)wglGetProcAddress("glConvolutionFilter2D");
			glConvolutionParameterf = (PFNGLCONVOLUTIONPARAMETERFPROC)wglGetProcAddress("glConvolutionParameterf");
			glConvolutionParameterfv = (PFNGLCONVOLUTIONPARAMETERFVPROC)wglGetProcAddress("glConvolutionParameterfv");
			glConvolutionParameteri = (PFNGLCONVOLUTIONPARAMETERIPROC)wglGetProcAddress("glConvolutionParameteri");
			glConvolutionParameteriv = (PFNGLCONVOLUTIONPARAMETERIVPROC)wglGetProcAddress("glConvolutionParameteriv");
			glCopyColorSubTable = (PFNGLCOPYCOLORSUBTABLEPROC)wglGetProcAddress("glCopyColorSubTable");
			glCopyColorTable = (PFNGLCOPYCOLORTABLEPROC)wglGetProcAddress("glCopyColorTable");
			glCopyConvolutionFilter1D = (PFNGLCOPYCONVOLUTIONFILTER1DPROC)wglGetProcAddress("glCopyConvolutionFilter1D");
			glCopyConvolutionFilter2D = (PFNGLCOPYCONVOLUTIONFILTER2DPROC)wglGetProcAddress("glCopyConvolutionFilter2D");
			glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)wglGetProcAddress("glCopyTexSubImage3D");
			glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");
			glGetColorTable = (PFNGLGETCOLORTABLEPROC)wglGetProcAddress("glGetColorTable");
			glGetColorTableParameterfv = (PFNGLGETCOLORTABLEPARAMETERFVPROC)wglGetProcAddress("glGetColorTableParameterfv");
			glGetColorTableParameteriv = (PFNGLGETCOLORTABLEPARAMETERIVPROC)wglGetProcAddress("glGetColorTableParameteriv");
			glGetConvolutionFilter = (PFNGLGETCONVOLUTIONFILTERPROC)wglGetProcAddress("glGetConvolutionFilter");
			glGetConvolutionParameterfv = (PFNGLGETCONVOLUTIONPARAMETERFVPROC)wglGetProcAddress("glGetConvolutionParameterfv");
			glGetConvolutionParameteriv = (PFNGLGETCONVOLUTIONPARAMETERIVPROC)wglGetProcAddress("glGetConvolutionParameteriv");
			glGetHistogram = (PFNGLGETHISTOGRAMPROC)wglGetProcAddress("glGetHistogram");
			glGetHistogramParameterfv = (PFNGLGETHISTOGRAMPARAMETERFVPROC)wglGetProcAddress("glGetHistogramParameterfv");
			glGetHistogramParameteriv = (PFNGLGETHISTOGRAMPARAMETERIVPROC)wglGetProcAddress("glGetHistogramParameteriv");
			glGetMinmax = (PFNGLGETMINMAXPROC)wglGetProcAddress("glGetMinmax");
			glGetMinmaxParameterfv = (PFNGLGETMINMAXPARAMETERFVPROC)wglGetProcAddress("glGetMinmaxParameterfv");
			glGetMinmaxParameteriv = (PFNGLGETMINMAXPARAMETERIVPROC)wglGetProcAddress("glGetMinmaxParameteriv");
			glGetSeparableFilter = (PFNGLGETSEPARABLEFILTERPROC)wglGetProcAddress("glGetSeparableFilter");
			glHistogram = (PFNGLHISTOGRAMPROC)wglGetProcAddress("glHistogram");
			glMinmax = (PFNGLMINMAXPROC)wglGetProcAddress("glMinmax");
			glResetHistogram = (PFNGLRESETHISTOGRAMPROC)wglGetProcAddress("glResetHistogram");
			glResetMinmax = (PFNGLRESETMINMAXPROC)wglGetProcAddress("glResetMinmax");
			glSeparableFilter2D = (PFNGLSEPARABLEFILTER2DPROC)wglGetProcAddress("glSeparableFilter2D");
			glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
			glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)wglGetProcAddress("glTexSubImage3D");
		}

		static void init_opengl_1_3(void)
		{
			init_opengl_1_2();

			glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
			glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)wglGetProcAddress("glClientActiveTexture");
			glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)wglGetProcAddress("glCompressedTexImage1D");
			glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2D");
			glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)wglGetProcAddress("glCompressedTexImage3D");
			glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)wglGetProcAddress("glCompressedTexSubImage1D");
			glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)wglGetProcAddress("glCompressedTexSubImage2D");
			glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)wglGetProcAddress("glCompressedTexSubImage3D");
			glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)wglGetProcAddress("glGetCompressedTexImage");
			glLoadTransposeMatrixd = (PFNGLLOADTRANSPOSEMATRIXDPROC)wglGetProcAddress("glLoadTransposeMatrixd");
			glLoadTransposeMatrixf = (PFNGLLOADTRANSPOSEMATRIXFPROC)wglGetProcAddress("glLoadTransposeMatrixf");
			glMultTransposeMatrixd = (PFNGLMULTTRANSPOSEMATRIXDPROC)wglGetProcAddress("glMultTransposeMatrixd");
			glMultTransposeMatrixf = (PFNGLMULTTRANSPOSEMATRIXFPROC)wglGetProcAddress("glMultTransposeMatrixf");
			glMultiTexCoord1d = (PFNGLMULTITEXCOORD1DPROC)wglGetProcAddress("glMultiTexCoord1d");
			glMultiTexCoord1dv = (PFNGLMULTITEXCOORD1DVPROC)wglGetProcAddress("glMultiTexCoord1dv");
			glMultiTexCoord1f = (PFNGLMULTITEXCOORD1FPROC)wglGetProcAddress("glMultiTexCoord1f");
			glMultiTexCoord1fv = (PFNGLMULTITEXCOORD1FVPROC)wglGetProcAddress("glMultiTexCoord1fv");
			glMultiTexCoord1i = (PFNGLMULTITEXCOORD1IPROC)wglGetProcAddress("glMultiTexCoord1i");
			glMultiTexCoord1iv = (PFNGLMULTITEXCOORD1IVPROC)wglGetProcAddress("glMultiTexCoord1iv");
			glMultiTexCoord1s = (PFNGLMULTITEXCOORD1SPROC)wglGetProcAddress("glMultiTexCoord1s");
			glMultiTexCoord1sv = (PFNGLMULTITEXCOORD1SVPROC)wglGetProcAddress("glMultiTexCoord1sv");
			glMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)wglGetProcAddress("glMultiTexCoord2d");
			glMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)wglGetProcAddress("glMultiTexCoord2dv");
			glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)wglGetProcAddress("glMultiTexCoord2f");
			glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)wglGetProcAddress("glMultiTexCoord2fv");
			glMultiTexCoord2i = (PFNGLMULTITEXCOORD2IPROC)wglGetProcAddress("glMultiTexCoord2i");
			glMultiTexCoord2iv = (PFNGLMULTITEXCOORD2IVPROC)wglGetProcAddress("glMultiTexCoord2iv");
			glMultiTexCoord2s = (PFNGLMULTITEXCOORD2SPROC)wglGetProcAddress("glMultiTexCoord2s");
			glMultiTexCoord2sv = (PFNGLMULTITEXCOORD2SVPROC)wglGetProcAddress("glMultiTexCoord2sv");
			glMultiTexCoord3d = (PFNGLMULTITEXCOORD3DPROC)wglGetProcAddress("glMultiTexCoord3d");
			glMultiTexCoord3dv = (PFNGLMULTITEXCOORD3DVPROC)wglGetProcAddress("glMultiTexCoord3dv");
			glMultiTexCoord3f = (PFNGLMULTITEXCOORD3FPROC)wglGetProcAddress("glMultiTexCoord3f");
			glMultiTexCoord3fv = (PFNGLMULTITEXCOORD3FVPROC)wglGetProcAddress("glMultiTexCoord3fv");
			glMultiTexCoord3i = (PFNGLMULTITEXCOORD3IPROC)wglGetProcAddress("glMultiTexCoord3i");
			glMultiTexCoord3iv = (PFNGLMULTITEXCOORD3IVPROC)wglGetProcAddress("glMultiTexCoord3iv");
			glMultiTexCoord3s = (PFNGLMULTITEXCOORD3SPROC)wglGetProcAddress("glMultiTexCoord3s");
			glMultiTexCoord3sv = (PFNGLMULTITEXCOORD3SVPROC)wglGetProcAddress("glMultiTexCoord3sv");
			glMultiTexCoord4d = (PFNGLMULTITEXCOORD4DPROC)wglGetProcAddress("glMultiTexCoord4d");
			glMultiTexCoord4dv = (PFNGLMULTITEXCOORD4DVPROC)wglGetProcAddress("glMultiTexCoord4dv");
			glMultiTexCoord4f = (PFNGLMULTITEXCOORD4FPROC)wglGetProcAddress("glMultiTexCoord4f");
			glMultiTexCoord4fv = (PFNGLMULTITEXCOORD4FVPROC)wglGetProcAddress("glMultiTexCoord4fv");
			glMultiTexCoord4i = (PFNGLMULTITEXCOORD4IPROC)wglGetProcAddress("glMultiTexCoord4i");
			glMultiTexCoord4iv = (PFNGLMULTITEXCOORD4IVPROC)wglGetProcAddress("glMultiTexCoord4iv");
			glMultiTexCoord4s = (PFNGLMULTITEXCOORD4SPROC)wglGetProcAddress("glMultiTexCoord4s");
			glMultiTexCoord4sv = (PFNGLMULTITEXCOORD4SVPROC)wglGetProcAddress("glMultiTexCoord4sv");
			glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)wglGetProcAddress("glSampleCoverage");
		}

		static void init_opengl_1_4(void)
		{
			init_opengl_1_3();

			glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
			glFogCoordPointer = (PFNGLFOGCOORDPOINTERPROC)wglGetProcAddress("glFogCoordPointer");
			glFogCoordd = (PFNGLFOGCOORDDPROC)wglGetProcAddress("glFogCoordd");
			glFogCoorddv = (PFNGLFOGCOORDDVPROC)wglGetProcAddress("glFogCoorddv");
			glFogCoordf = (PFNGLFOGCOORDFPROC)wglGetProcAddress("glFogCoordf");
			glFogCoordfv = (PFNGLFOGCOORDFVPROC)wglGetProcAddress("glFogCoordfv");
			glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)wglGetProcAddress("glMultiDrawArrays");
			glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)wglGetProcAddress("glMultiDrawElements");
			glPointParameterf = (PFNGLPOINTPARAMETERFPROC)wglGetProcAddress("glPointParameterf");
			glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)wglGetProcAddress("glPointParameterfv");
			glPointParameteri = (PFNGLPOINTPARAMETERIPROC)wglGetProcAddress("glPointParameteri");
			glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)wglGetProcAddress("glPointParameteriv");
			glSecondaryColor3b = (PFNGLSECONDARYCOLOR3BPROC)wglGetProcAddress("glSecondaryColor3b");
			glSecondaryColor3bv = (PFNGLSECONDARYCOLOR3BVPROC)wglGetProcAddress("glSecondaryColor3bv");
			glSecondaryColor3d = (PFNGLSECONDARYCOLOR3DPROC)wglGetProcAddress("glSecondaryColor3d");
			glSecondaryColor3dv = (PFNGLSECONDARYCOLOR3DVPROC)wglGetProcAddress("glSecondaryColor3dv");
			glSecondaryColor3f = (PFNGLSECONDARYCOLOR3FPROC)wglGetProcAddress("glSecondaryColor3f");
			glSecondaryColor3fv = (PFNGLSECONDARYCOLOR3FVPROC)wglGetProcAddress("glSecondaryColor3fv");
			glSecondaryColor3i = (PFNGLSECONDARYCOLOR3IPROC)wglGetProcAddress("glSecondaryColor3i");
			glSecondaryColor3iv = (PFNGLSECONDARYCOLOR3IVPROC)wglGetProcAddress("glSecondaryColor3iv");
			glSecondaryColor3s = (PFNGLSECONDARYCOLOR3SPROC)wglGetProcAddress("glSecondaryColor3s");
			glSecondaryColor3sv = (PFNGLSECONDARYCOLOR3SVPROC)wglGetProcAddress("glSecondaryColor3sv");
			glSecondaryColor3ub = (PFNGLSECONDARYCOLOR3UBPROC)wglGetProcAddress("glSecondaryColor3ub");
			glSecondaryColor3ubv = (PFNGLSECONDARYCOLOR3UBVPROC)wglGetProcAddress("glSecondaryColor3ubv");
			glSecondaryColor3ui = (PFNGLSECONDARYCOLOR3UIPROC)wglGetProcAddress("glSecondaryColor3ui");
			glSecondaryColor3uiv = (PFNGLSECONDARYCOLOR3UIVPROC)wglGetProcAddress("glSecondaryColor3uiv");
			glSecondaryColor3us = (PFNGLSECONDARYCOLOR3USPROC)wglGetProcAddress("glSecondaryColor3us");
			glSecondaryColor3usv = (PFNGLSECONDARYCOLOR3USVPROC)wglGetProcAddress("glSecondaryColor3usv");
			glSecondaryColorPointer = (PFNGLSECONDARYCOLORPOINTERPROC)wglGetProcAddress("glSecondaryColorPointer");
			glWindowPos2d = (PFNGLWINDOWPOS2DPROC)wglGetProcAddress("glWindowPos2d");
			glWindowPos2dv = (PFNGLWINDOWPOS2DVPROC)wglGetProcAddress("glWindowPos2dv");
			glWindowPos2f = (PFNGLWINDOWPOS2FPROC)wglGetProcAddress("glWindowPos2f");
			glWindowPos2fv = (PFNGLWINDOWPOS2FVPROC)wglGetProcAddress("glWindowPos2fv");
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i");
			glWindowPos2iv = (PFNGLWINDOWPOS2IVPROC)wglGetProcAddress("glWindowPos2iv");
			glWindowPos2s = (PFNGLWINDOWPOS2SPROC)wglGetProcAddress("glWindowPos2s");
			glWindowPos2sv = (PFNGLWINDOWPOS2SVPROC)wglGetProcAddress("glWindowPos2sv");
			glWindowPos3d = (PFNGLWINDOWPOS3DPROC)wglGetProcAddress("glWindowPos3d");
			glWindowPos3dv = (PFNGLWINDOWPOS3DVPROC)wglGetProcAddress("glWindowPos3dv");
			glWindowPos3f = (PFNGLWINDOWPOS3FPROC)wglGetProcAddress("glWindowPos3f");
			glWindowPos3fv = (PFNGLWINDOWPOS3FVPROC)wglGetProcAddress("glWindowPos3fv");
			glWindowPos3i = (PFNGLWINDOWPOS3IPROC)wglGetProcAddress("glWindowPos3i");
			glWindowPos3iv = (PFNGLWINDOWPOS3IVPROC)wglGetProcAddress("glWindowPos3iv");
			glWindowPos3s = (PFNGLWINDOWPOS3SPROC)wglGetProcAddress("glWindowPos3s");
			glWindowPos3sv = (PFNGLWINDOWPOS3SVPROC)wglGetProcAddress("glWindowPos3sv");
		}

		bool extension_support(const string &extension)
		{
			return (extensions.find(extension) != extensions.end());
		}

		bool GL_ARB_vertex_program_initialized = false;
		bool init_GL_ARB_vertex_program(void)
		{
			if (!extension_support("GL_ARB_vertex_program")) return false;

			glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
			glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARBPROC)wglGetProcAddress("glDeleteProgramsARB");
			glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");
			glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
			glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
			glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramEnvParameterdvARB");
			glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramEnvParameterfvARB");
			glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramLocalParameterdvARB");
			glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramLocalParameterfvARB");
			glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARBPROC)wglGetProcAddress("glGetProgramStringARB");
			glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)wglGetProcAddress("glGetProgramivARB");
			glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)wglGetProcAddress("glGetVertexAttribPointervARB");
			glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARBPROC)wglGetProcAddress("glGetVertexAttribdvARB");
			glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARBPROC)wglGetProcAddress("glGetVertexAttribfvARB");
			glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARBPROC)wglGetProcAddress("glGetVertexAttribivARB");
			glIsProgramARB = (PFNGLISPROGRAMARBPROC)wglGetProcAddress("glIsProgramARB");
			glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARBPROC)wglGetProcAddress("glProgramEnvParameter4dARB");
			glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARBPROC)wglGetProcAddress("glProgramEnvParameter4dvARB");
			glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARBPROC)wglGetProcAddress("glProgramEnvParameter4fARB");
			glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARBPROC)wglGetProcAddress("glProgramEnvParameter4fvARB");
			glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARBPROC)wglGetProcAddress("glProgramLocalParameter4dARB");
			glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)wglGetProcAddress("glProgramLocalParameter4dvARB");
			glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)wglGetProcAddress("glProgramLocalParameter4fARB");
			glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)wglGetProcAddress("glProgramLocalParameter4fvARB");
			glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
			glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARBPROC)wglGetProcAddress("glVertexAttrib1dARB");
			glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARBPROC)wglGetProcAddress("glVertexAttrib1dvARB");
			glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARBPROC)wglGetProcAddress("glVertexAttrib1fARB");
			glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARBPROC)wglGetProcAddress("glVertexAttrib1fvARB");
			glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARBPROC)wglGetProcAddress("glVertexAttrib1sARB");
			glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARBPROC)wglGetProcAddress("glVertexAttrib1svARB");
			glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARBPROC)wglGetProcAddress("glVertexAttrib2dARB");
			glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARBPROC)wglGetProcAddress("glVertexAttrib2dvARB");
			glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARBPROC)wglGetProcAddress("glVertexAttrib2fARB");
			glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARBPROC)wglGetProcAddress("glVertexAttrib2fvARB");
			glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARBPROC)wglGetProcAddress("glVertexAttrib2sARB");
			glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARBPROC)wglGetProcAddress("glVertexAttrib2svARB");
			glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARBPROC)wglGetProcAddress("glVertexAttrib3dARB");
			glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARBPROC)wglGetProcAddress("glVertexAttrib3dvARB");
			glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");
			glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC)wglGetProcAddress("glVertexAttrib3fvARB");
			glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARBPROC)wglGetProcAddress("glVertexAttrib3sARB");
			glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARBPROC)wglGetProcAddress("glVertexAttrib3svARB");
			glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARBPROC)wglGetProcAddress("glVertexAttrib4NbvARB");
			glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARBPROC)wglGetProcAddress("glVertexAttrib4NivARB");
			glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARBPROC)wglGetProcAddress("glVertexAttrib4NsvARB");
			glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARBPROC)wglGetProcAddress("glVertexAttrib4NubARB");
			glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARBPROC)wglGetProcAddress("glVertexAttrib4NubvARB");
			glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARBPROC)wglGetProcAddress("glVertexAttrib4NuivARB");
			glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARBPROC)wglGetProcAddress("glVertexAttrib4NusvARB");
			glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARBPROC)wglGetProcAddress("glVertexAttrib4bvARB");
			glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARBPROC)wglGetProcAddress("glVertexAttrib4dARB");
			glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARBPROC)wglGetProcAddress("glVertexAttrib4dvARB");
			glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARBPROC)wglGetProcAddress("glVertexAttrib4fARB");
			glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARBPROC)wglGetProcAddress("glVertexAttrib4fvARB");
			glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARBPROC)wglGetProcAddress("glVertexAttrib4ivARB");
			glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARBPROC)wglGetProcAddress("glVertexAttrib4sARB");
			glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARBPROC)wglGetProcAddress("glVertexAttrib4svARB");
			glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARBPROC)wglGetProcAddress("glVertexAttrib4ubvARB");
			glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARBPROC)wglGetProcAddress("glVertexAttrib4uivARB");
			glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARBPROC)wglGetProcAddress("glVertexAttrib4usvARB");
			glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");

			GL_ARB_vertex_program_initialized = true;
			return true;
		}

		bool GL_ARB_fragment_program_initialized = false;
		bool init_GL_ARB_fragment_program(void)
		{
			if (!extension_support("GL_ARB_fragment_program")) return false;
			if (!GL_ARB_vertex_program_initialized && !init_GL_ARB_vertex_program()) return false;
			GL_ARB_fragment_program_initialized = true;
			return true;
		}

		bool WGL_ARB_extensions_string_initialized = false;
		bool init_WGL_ARB_extensions_string(void)
		{
			wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
			if (!wglGetExtensionsStringARB) return false;

			WGL_ARB_extensions_string_initialized = true;
			return true;
		}

		bool WGL_ARB_pixel_format_initialized = false;
		bool init_WGL_ARB_pixel_format(void)
		{
			if (!extension_support("WGL_ARB_pixel_format")) return false;

			wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
			wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");

			WGL_ARB_pixel_format_initialized = true;
			return true;
		}

		bool WGL_ARB_make_current_read_initialized = false;
		bool init_WGL_ARB_make_current_read(void)
		{
			if (!extension_support("WGL_ARB_make_current_read")) return false;

			wglGetCurrentReadDCARB = (PFNWGLGETCURRENTREADDCARBPROC)wglGetProcAddress("wglGetCurrentReadDCARB");
			wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC)wglGetProcAddress("wglMakeContextCurrentARB");

			WGL_ARB_make_current_read_initialized = true;
			return true;
		}

		bool WGL_ARB_pbuffer_initialized = false;
		bool init_WGL_ARB_pbuffer(void)
		{
			if (!extension_support("WGL_ARB_pbuffer")) return false;

			wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
			wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
			wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
			wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
			wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");

			WGL_ARB_pbuffer_initialized = true;
			return true;
		}

		bool WGL_ARB_render_texture_initialized = false;
		bool init_WGL_ARB_render_texture(void)
		{
			if (!extension_support("WGL_ARB_render_texture")) return false;

			wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
			wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
			wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");

			WGL_ARB_render_texture_initialized = true;
			return true;
		}

		bool GL_EXT_stencil_two_side_initialized = false;
		bool init_GL_EXT_stencil_two_side(void)
		{
			if (!extension_support("GL_EXT_stencil_two_side")) return false;

			glActiveStencilFaceEXT = (PFNGLACTIVESTENCILFACEEXTPROC)wglGetProcAddress("glActiveStencilFaceEXT");

			GL_EXT_stencil_two_side_initialized = true;
			return true;
		}

		bool WGL_EXT_extensions_string_initialized = false;
		bool init_WGL_EXT_extensions_string(void)
		{
			wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
			if (!wglGetExtensionsStringEXT) return false;

			WGL_EXT_extensions_string_initialized = true;
			return true;
		}

		bool GL_ATI_separate_stencil_initialized = false;
		bool init_GL_ATI_separate_stencil(void)
		{
			if (!extension_support("GL_ATI_separate_stencil")) return false;

			glStencilOpSeparateATI = (PFNGLSTENCILOPSEPARATEATIPROC)wglGetProcAddress("glStencilOpSeparateATI");
			glStencilFuncSeparateATI = (PFNGLSTENCILFUNCSEPARATEATIPROC)wglGetProcAddress("glStencilFuncSeparateATI");

			GL_ATI_separate_stencil_initialized = true;
			return true;
		}

		bool WGL_ATI_pixel_format_float_initialized = false;
		bool init_WGL_ATI_pixel_format_float(void)
		{
			if (!extension_support("WGL_ATI_pixel_format_float")) return false;
			WGL_ATI_pixel_format_float_initialized = true;
			return true;
		}

		bool GL_EXT_texture_filter_anisotropic_initialized = false;
		bool init_GL_EXT_texture_filter_anisotropic(void)
		{
			if (!extension_support("GL_EXT_texture_filter_anisotropic")) return false;
			GL_EXT_texture_filter_anisotropic_initialized = true;
			return true;
		}

		bool GL_NV_texture_rectangle_initialized = false;
		bool init_GL_NV_texture_rectangle(void)
		{
			if (!extension_support("GL_NV_texture_rectangle")) return false;
			GL_NV_texture_rectangle_initialized = true;
			return true;
		}

		bool GL_NV_float_buffer_initialized = false;
		bool init_GL_NV_float_buffer(void)
		{
			if (!extension_support("GL_NV_float_buffer")) return false;
			GL_NV_float_buffer_initialized = true;
			return true;
		}

		bool WGL_NV_render_texture_rectangle_initialized = false;
		bool init_WGL_NV_render_texture_rectangle(void)
		{
			if (!extension_support("WGL_NV_render_texture_rectangle")) return false;
			WGL_NV_render_texture_rectangle_initialized = true;
			return true;
		}

		bool WGL_NV_float_buffer_initialized = false;
		bool init_WGL_NV_float_buffer(void)
		{
			if (!extension_support("WGL_NV_float_buffer")) return false;
			WGL_NV_float_buffer_initialized = true;
			return true;
		}

		void init_opengl(void)
		{
			init_opengl_1_4();

			string extension;
			istringstream extensionstream((const char *)glGetString(GL_EXTENSIONS));

			while (extensionstream >> extension) extensions.insert(extension);

			if (init_WGL_ARB_extensions_string()) {
				istringstream wglextensionstream((const char *)wglGetExtensionsStringARB(platform::getDC()));

				while (wglextensionstream >> extension) extensions.insert(extension);
			}
			else if (init_WGL_EXT_extensions_string()) {
				istringstream wglextensionstream((const char *)wglGetExtensionsStringEXT());

				while (wglextensionstream >> extension) extensions.insert(extension);
			}

			for (set<string>::iterator it = extensions.begin(); it != extensions.end(); ++it) {
				console::print(0,1,0,*it);
			}
		}
	}
}