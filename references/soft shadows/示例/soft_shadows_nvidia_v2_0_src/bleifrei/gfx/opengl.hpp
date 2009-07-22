
#ifndef _include_bleifrei_opengl_hpp_
#define _include_bleifrei_opengl_hpp_

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"
#include "wglext.h"
#include "glATI.h"
#include "wglATI.h"

// OpenGL 1.2
extern PFNGLBLENDCOLORPROC glBlendColor;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLCOLORSUBTABLEPROC glColorSubTable;
extern PFNGLCOLORTABLEPROC glColorTable;
extern PFNGLCOLORTABLEPARAMETERFVPROC glColorTableParameterfv;
extern PFNGLCOLORTABLEPARAMETERIVPROC glColorTableParameteriv;
extern PFNGLCONVOLUTIONFILTER1DPROC glConvolutionFilter1D;
extern PFNGLCONVOLUTIONFILTER2DPROC glConvolutionFilter2D;
extern PFNGLCONVOLUTIONPARAMETERFPROC glConvolutionParameterf;
extern PFNGLCONVOLUTIONPARAMETERFVPROC glConvolutionParameterfv;
extern PFNGLCONVOLUTIONPARAMETERIPROC glConvolutionParameteri;
extern PFNGLCONVOLUTIONPARAMETERIVPROC glConvolutionParameteriv;
extern PFNGLCOPYCOLORSUBTABLEPROC glCopyColorSubTable;
extern PFNGLCOPYCOLORTABLEPROC glCopyColorTable;
extern PFNGLCOPYCONVOLUTIONFILTER1DPROC glCopyConvolutionFilter1D;
extern PFNGLCOPYCONVOLUTIONFILTER2DPROC glCopyConvolutionFilter2D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLGETCOLORTABLEPROC glGetColorTable;
extern PFNGLGETCOLORTABLEPARAMETERFVPROC glGetColorTableParameterfv;
extern PFNGLGETCOLORTABLEPARAMETERIVPROC glGetColorTableParameteriv;
extern PFNGLGETCONVOLUTIONFILTERPROC glGetConvolutionFilter;
extern PFNGLGETCONVOLUTIONPARAMETERFVPROC glGetConvolutionParameterfv;
extern PFNGLGETCONVOLUTIONPARAMETERIVPROC glGetConvolutionParameteriv;
extern PFNGLGETHISTOGRAMPROC glGetHistogram;
extern PFNGLGETHISTOGRAMPARAMETERFVPROC glGetHistogramParameterfv;
extern PFNGLGETHISTOGRAMPARAMETERIVPROC glGetHistogramParameteriv;
extern PFNGLGETMINMAXPROC glGetMinmax;
extern PFNGLGETMINMAXPARAMETERFVPROC glGetMinmaxParameterfv;
extern PFNGLGETMINMAXPARAMETERIVPROC glGetMinmaxParameteriv;
extern PFNGLGETSEPARABLEFILTERPROC glGetSeparableFilter;
extern PFNGLHISTOGRAMPROC glHistogram;
extern PFNGLMINMAXPROC glMinmax;
extern PFNGLRESETHISTOGRAMPROC glResetHistogram;
extern PFNGLRESETMINMAXPROC glResetMinmax;
extern PFNGLSEPARABLEFILTER2DPROC glSeparableFilter2D;
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;

// OpenGL 1.3
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
extern PFNGLLOADTRANSPOSEMATRIXDPROC glLoadTransposeMatrixd;
extern PFNGLLOADTRANSPOSEMATRIXFPROC glLoadTransposeMatrixf;
extern PFNGLMULTTRANSPOSEMATRIXDPROC glMultTransposeMatrixd;
extern PFNGLMULTTRANSPOSEMATRIXFPROC glMultTransposeMatrixf;
extern PFNGLMULTITEXCOORD1DPROC glMultiTexCoord1d;
extern PFNGLMULTITEXCOORD1DVPROC glMultiTexCoord1dv;
extern PFNGLMULTITEXCOORD1FPROC glMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVPROC glMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD1IPROC glMultiTexCoord1i;
extern PFNGLMULTITEXCOORD1IVPROC glMultiTexCoord1iv;
extern PFNGLMULTITEXCOORD1SPROC glMultiTexCoord1s;
extern PFNGLMULTITEXCOORD1SVPROC glMultiTexCoord1sv;
extern PFNGLMULTITEXCOORD2DPROC glMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC glMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD2IPROC glMultiTexCoord2i;
extern PFNGLMULTITEXCOORD2IVPROC glMultiTexCoord2iv;
extern PFNGLMULTITEXCOORD2SPROC glMultiTexCoord2s;
extern PFNGLMULTITEXCOORD2SVPROC glMultiTexCoord2sv;
extern PFNGLMULTITEXCOORD3DPROC glMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC glMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD3FPROC glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC glMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD3IPROC glMultiTexCoord3i;
extern PFNGLMULTITEXCOORD3IVPROC glMultiTexCoord3iv;
extern PFNGLMULTITEXCOORD3SPROC glMultiTexCoord3s;
extern PFNGLMULTITEXCOORD3SVPROC glMultiTexCoord3sv;
extern PFNGLMULTITEXCOORD4DPROC glMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC glMultiTexCoord4dv;
extern PFNGLMULTITEXCOORD4FPROC glMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC glMultiTexCoord4fv;
extern PFNGLMULTITEXCOORD4IPROC glMultiTexCoord4i;
extern PFNGLMULTITEXCOORD4IVPROC glMultiTexCoord4iv;
extern PFNGLMULTITEXCOORD4SPROC glMultiTexCoord4s;
extern PFNGLMULTITEXCOORD4SVPROC glMultiTexCoord4sv;
extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;

// OpenGL 1.4
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLFOGCOORDPOINTERPROC glFogCoordPointer;
extern PFNGLFOGCOORDDPROC glFogCoordd;
extern PFNGLFOGCOORDDVPROC glFogCoorddv;
extern PFNGLFOGCOORDFPROC glFogCoordf;
extern PFNGLFOGCOORDFVPROC glFogCoordfv;
extern PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC glPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
extern PFNGLSECONDARYCOLOR3BPROC glSecondaryColor3b;
extern PFNGLSECONDARYCOLOR3BVPROC glSecondaryColor3bv;
extern PFNGLSECONDARYCOLOR3DPROC glSecondaryColor3d;
extern PFNGLSECONDARYCOLOR3DVPROC glSecondaryColor3dv;
extern PFNGLSECONDARYCOLOR3FPROC glSecondaryColor3f;
extern PFNGLSECONDARYCOLOR3FVPROC glSecondaryColor3fv;
extern PFNGLSECONDARYCOLOR3IPROC glSecondaryColor3i;
extern PFNGLSECONDARYCOLOR3IVPROC glSecondaryColor3iv;
extern PFNGLSECONDARYCOLOR3SPROC glSecondaryColor3s;
extern PFNGLSECONDARYCOLOR3SVPROC glSecondaryColor3sv;
extern PFNGLSECONDARYCOLOR3UBPROC glSecondaryColor3ub;
extern PFNGLSECONDARYCOLOR3UBVPROC glSecondaryColor3ubv;
extern PFNGLSECONDARYCOLOR3UIPROC glSecondaryColor3ui;
extern PFNGLSECONDARYCOLOR3UIVPROC glSecondaryColor3uiv;
extern PFNGLSECONDARYCOLOR3USPROC glSecondaryColor3us;
extern PFNGLSECONDARYCOLOR3USVPROC glSecondaryColor3usv;
extern PFNGLSECONDARYCOLORPOINTERPROC glSecondaryColorPointer;
extern PFNGLWINDOWPOS2DPROC glWindowPos2d;
extern PFNGLWINDOWPOS2DVPROC glWindowPos2dv;
extern PFNGLWINDOWPOS2FPROC glWindowPos2f;
extern PFNGLWINDOWPOS2FVPROC glWindowPos2fv;
extern PFNGLWINDOWPOS2IPROC glWindowPos2i;
extern PFNGLWINDOWPOS2IVPROC glWindowPos2iv;
extern PFNGLWINDOWPOS2SPROC glWindowPos2s;
extern PFNGLWINDOWPOS2SVPROC glWindowPos2sv;
extern PFNGLWINDOWPOS3DPROC glWindowPos3d;
extern PFNGLWINDOWPOS3DVPROC glWindowPos3dv;
extern PFNGLWINDOWPOS3FPROC glWindowPos3f;
extern PFNGLWINDOWPOS3FVPROC glWindowPos3fv;
extern PFNGLWINDOWPOS3IPROC glWindowPos3i;
extern PFNGLWINDOWPOS3IVPROC glWindowPos3iv;
extern PFNGLWINDOWPOS3SPROC glWindowPos3s;
extern PFNGLWINDOWPOS3SVPROC glWindowPos3sv;

// GL_ARB_vertex_program
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB;
extern PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
extern PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB;
extern PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB;
extern PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB;
extern PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB;
extern PFNGLISPROGRAMARBPROC glIsProgramARB;
extern PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB;
extern PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
extern PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
extern PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB;
extern PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB;
extern PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB;
extern PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB;
extern PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB;
extern PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB;
extern PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB;
extern PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB;
extern PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB;
extern PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB;
extern PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB;
extern PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB;
extern PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB;
extern PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB;
extern PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB;
extern PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB;
extern PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB;
extern PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB;
extern PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB;
extern PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB;
extern PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB;
extern PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB;
extern PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB;
extern PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB;
extern PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB;
extern PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB;
extern PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB;
extern PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB;
extern PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB;
extern PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB;

// GL_EXT_stencil_two_side
extern PFNGLACTIVESTENCILFACEEXTPROC glActiveStencilFaceEXT;

// GL_ATI_separate_stencil
extern PFNGLSTENCILOPSEPARATEATIPROC glStencilOpSeparateATI;
extern PFNGLSTENCILFUNCSEPARATEATIPROC glStencilFuncSeparateATI;

// WGL_ARB_extensions_string
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

// WGL_ARB_pixel_format
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;

// WGL_ARB_make_current_read
extern PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB;
extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;

// WGL_ARB_pbuffer
extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
extern PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;

// WGL_ARB_render_texture
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;

// WGL_EXT_extensions_string
extern PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT;

namespace bleifrei {
	namespace gfx {
		bool extension_support(const std::string &extension);

		bool init_GL_ARB_vertex_program(void);
		extern bool GL_ARB_vertex_program_initialized;

		bool init_GL_ARB_fragment_program(void);
		extern bool GL_ARB_fragment_program_initialized;

		bool init_WGL_ARB_extensions_string(void);
		extern bool WGL_ARB_extensions_string_initialized;

		bool init_WGL_ARB_pixel_format(void);
		extern bool WGL_ARB_pixel_format_initialized;

		bool init_WGL_ARB_make_current_read(void);
		extern bool WGL_ARB_make_current_read_initialized;

		bool init_WGL_ARB_pbuffer(void);
		extern bool WGL_ARB_pbuffer_initialized;

		bool init_WGL_ARB_render_texture(void);
		extern bool WGL_ARB_render_texture_initialized;

		bool init_GL_EXT_texture_filter_anisotropic(void);
		extern bool GL_EXT_texture_filter_anisotropic_initialized;

		bool init_GL_EXT_stencil_two_side(void);
		extern bool GL_EXT_stencil_two_side_initialized;

		bool init_WGL_EXT_extensions_string(void);
		extern bool WGL_EXT_extensions_string_initialized;

		bool init_GL_ATI_separate_stencil(void);
		extern bool GL_ATI_separate_stencil_initialized;

		bool init_WGL_ATI_pixel_format_float(void);
		extern bool WGL_ATI_pixel_format_float_initialized;

		bool init_GL_NV_texture_rectangle(void);
		extern bool GL_NV_texture_rectangle_initialized;

		bool init_GL_NV_float_buffer(void);
		extern bool GL_NV_float_buffer_initialized;

		bool init_WGL_NV_render_texture_rectangle(void);
		extern bool WGL_NV_render_texture_rectangle_initialized;

		bool init_WGL_NV_float_buffer(void);
		extern bool WGL_NV_float_buffer_initialized;

		void init_opengl(void);
	}
}

#endif //_include_bleifrei_opengl_hpp_
