
// includes
#include "../io/console.hpp"
#include "../platform/platform.hpp"

#include "RenderTarget.hpp"

using namespace bleifrei::io;
using namespace bleifrei;

namespace bleifrei {
	namespace gfx {

		RenderTarget *RenderTarget::active = 0;

		RenderTarget::RenderTarget(int width, int height, Format format)
		{
			this->width = width;
			this->height = height;

			unsigned int count = 0;
			int			 pixelFormat;

			if (!WGL_ARB_pixel_format_initialized) {
				console::print(1, 0, 0, "Error(RenderTarget): WGL_ARB_pixel_format not avilable!");
				return;
			}
			/*if (!WGL_ARB_make_current_read_initialized) {
				console::print(1, 0, 0, "Error(RenderTarget): WGL_ARB_make_current_read not avilable!");
				return;
			}*/
			if (!WGL_ARB_pbuffer_initialized) {
				console::print(1, 0, 0, "Error(RenderTarget): WGL_ARB_pbuffer not avilable!");
				return;
			}
			if (!WGL_ARB_render_texture_initialized) {
				console::print(1, 0, 0, "Error(RenderTarget): WGL_ARB_render_texture not avilable!");
				return;
			}

			int pAttrib[] = {
				WGL_TEXTURE_FORMAT_ARB,		WGL_TEXTURE_RGBA_ARB,
				WGL_TEXTURE_TARGET_ARB,		WGL_TEXTURE_2D_ARB,
				0
			};

			if (format == rgba8) {
				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,				TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,			TRUE,
					WGL_BIND_TO_TEXTURE_RGBA_ARB,		TRUE,
					WGL_RED_BITS_ARB,					8,
					WGL_GREEN_BITS_ARB,					8,
					WGL_BLUE_BITS_ARB,					8,
					WGL_ALPHA_BITS_ARB,					8,
					WGL_DEPTH_BITS_ARB,					24,
					WGL_DOUBLE_BUFFER_ARB,				FALSE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1, 0, 0, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}
			}
			else if (format == rgba8rect) {
				if (!WGL_NV_render_texture_rectangle_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): WGL_NV_render_texture_rectangle not avilable!");
					return;
				}

				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,				TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,			TRUE,
					WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV,	TRUE,
					WGL_RED_BITS_ARB,					8,
					WGL_GREEN_BITS_ARB,					8,
					WGL_BLUE_BITS_ARB,					8,
					WGL_ALPHA_BITS_ARB,					8,
					WGL_DEPTH_BITS_ARB,					24,
					WGL_DOUBLE_BUFFER_ARB,				FALSE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1, 0, 0, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[3] = WGL_TEXTURE_RECTANGLE_NV;
			}
			else if (format == rgb8) {
				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,				TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,			TRUE,
					WGL_BIND_TO_TEXTURE_RGBA_ARB,		TRUE,
					WGL_RED_BITS_ARB,					8,
					WGL_GREEN_BITS_ARB,					8,
					WGL_BLUE_BITS_ARB,					8,
					WGL_DEPTH_BITS_ARB,					24,
					WGL_DOUBLE_BUFFER_ARB,				FALSE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1, 0, 0, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[1] = WGL_TEXTURE_RGB_ARB;
			}
			else if (format == rgba8db) {
				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,				TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,			TRUE,
					WGL_BIND_TO_TEXTURE_RGBA_ARB,		TRUE,
					WGL_RED_BITS_ARB,					8,
					WGL_GREEN_BITS_ARB,					8,
					WGL_BLUE_BITS_ARB,					8,
					WGL_ALPHA_BITS_ARB,					8,
					WGL_DEPTH_BITS_ARB,					24,
					WGL_DOUBLE_BUFFER_ARB,				TRUE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1, 0, 0, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}
			}
			else if (format == rgba8dbrect) {
				if (!WGL_NV_render_texture_rectangle_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): WGL_NV_render_texture_rectangle not avilable!");
					return;
				}

				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,				TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,			TRUE,
					WGL_BIND_TO_TEXTURE_RECTANGLE_RGBA_NV,	TRUE,
					WGL_RED_BITS_ARB,					8,
					WGL_GREEN_BITS_ARB,					8,
					WGL_BLUE_BITS_ARB,					8,
					WGL_ALPHA_BITS_ARB,					8,
					WGL_DEPTH_BITS_ARB,					24,
					WGL_STENCIL_BITS_ARB,				8,
					WGL_DOUBLE_BUFFER_ARB,				TRUE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1, 0, 0, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[3] = WGL_TEXTURE_RECTANGLE_NV;
			}
			else if (format == rgb16f) {
				if (!WGL_ATI_pixel_format_float_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): WGL_ATI_pixel_format_float not avilable!");
					return;
				}
				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,			TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,		TRUE,
					WGL_BIND_TO_TEXTURE_RGB_ARB,	TRUE,
					WGL_PIXEL_TYPE_ARB,				WGL_TYPE_RGBA_FLOAT_ATI,
					WGL_RED_BITS_ARB,				32,
					WGL_GREEN_BITS_ARB,				32,
					WGL_BLUE_BITS_ARB,				32,
					WGL_DEPTH_BITS_ARB,				24,
					WGL_DOUBLE_BUFFER_ARB,			FALSE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[1] = WGL_TEXTURE_RGB_ARB;
			}
			else if (format == rgb16frect) {
				if (!GL_NV_float_buffer_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): GL_NV_float_buffer not avilable!");
					return;
				}
				if (!WGL_NV_float_buffer_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): WGL_NV_float_buffer not avilable!");
					return;
				}
				if (!WGL_NV_render_texture_rectangle_initialized) {
					console::print(1, 0, 0, "Error(RenderTarget): WGL_NV_render_texture_rectangle not avilable!");
					return;
				}

				int attr[] = {
					WGL_SUPPORT_OPENGL_ARB,			TRUE,
					WGL_DRAW_TO_PBUFFER_ARB,		TRUE,
					WGL_BIND_TO_TEXTURE_RECTANGLE_FLOAT_RGB_NV, TRUE,
					WGL_FLOAT_COMPONENTS_NV,					TRUE,
					WGL_RED_BITS_ARB,				32,
					WGL_GREEN_BITS_ARB,				32,
					WGL_BLUE_BITS_ARB,				32,
					WGL_DEPTH_BITS_ARB,				24,
					WGL_DOUBLE_BUFFER_ARB,			FALSE,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[1] = WGL_TEXTURE_FLOAT_RGB_NV;
				pAttrib[3] = WGL_TEXTURE_RECTANGLE_NV;
			}
			else {
				console::print(1, 0, 0, "Error(RenderTarget): Unknown format!");
			}

			if (count == 0) {
				console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not find suitable pbuffer pixel format!");
				return;
			}

			pb = wglCreatePbufferARB(platform::getDC(), pixelFormat, width, height, pAttrib);

			if (!pb) {
				console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not create pbuffer handle!");
				return;
			}

			dc = wglGetPbufferDCARB(pb);

			if (!dc) {
				console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not create pbuffer device context!");
				return;
			}

			rc = wglCreateContext(dc);

			if (!rc) {
				console::print(1.0f, 0.0f, 0.0f, "Error(RenderTarget): Could not create pbuffer RC!");
				return;
			}

			wglShareLists(platform::getRC(), rc);

			setActive(this);
				platform::forceResize(width, height);

				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glShadeModel(GL_SMOOTH);
				glEnable(GL_CULL_FACE);
			setActive(0);

			int w, h;
			wglQueryPbufferARB(pb, WGL_PBUFFER_WIDTH_ARB, &w);
			wglQueryPbufferARB(pb, WGL_PBUFFER_HEIGHT_ARB, &h);
			console::printf(1,1,0, "Info(RenderTarget): width = %i, height = %i", w, h);
		}

		RenderTarget::~RenderTarget(void)
		{
		    wglReleasePbufferDCARB(pb, dc);
			wglDestroyPbufferARB(pb);
		}

		void RenderTarget::enableOrtho(void)
		{
			enableOrtho(width, height);
		}

		void RenderTarget::activate(void)
		{
			//wglMakeCurrent(dc, platform::getRC()); //wglGetCurrentContext());
			wglMakeCurrent(dc, rc); //wglMakeContextCurrentARB(dc, dc, platform::getRC());

			glDrawBuffer(GL_FRONT);
			glReadBuffer(GL_FRONT);

			glViewport(0, 0, width, height);
		}

		void RenderTarget::setActive(RenderTarget *target)
		{
			if (target != active) {
				if (target == 0) {
					wglMakeCurrent(platform::getDC(), platform::getRC());

					glDrawBuffer(GL_BACK);
					glReadBuffer(GL_BACK);
					
					platform::forceResize(platform::getScreenWidth(), platform::getScreenHeight());
				}
				else {
					target->activate();
				}
				active = target;
			}
		}

		void RenderTarget::enableOrtho(int width, int height) 
		{
			glPushMatrix();
			glLoadIdentity();

			glMatrixMode(GL_PROJECTION);
			glPushMatrix();

			glLoadIdentity();
			glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

			glMatrixMode(GL_MODELVIEW);
		}

		void RenderTarget::disableOrtho(void)
		{
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

	}
}

