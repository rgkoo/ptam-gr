/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#include <iostream>

// includes
#include "../io/console.hpp"
#include "../gfx/opengl.hpp"
#include "../platform/platform.hpp"

#include "PixelBuffer.hpp"

using namespace bleifrei::io;
using namespace bleifrei;
using namespace std;

namespace bleifrei {
	namespace gfx {

		PixelBuffer::PixelBuffer(const PixelBuffer &p)
		{
			width	= p.width;
			height	= p.height;
			
			pb		= p.pb;
			dc		= p.dc;
		}

		PixelBuffer::PixelBuffer(int width, int height, Format format)
		{
			this->width = width;
			this->height = height;

			unsigned int count = 0;
			int			 pixelFormat;

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
					//WGL_AUX_BUFFERS_ARB,				1,
					0
				};

				if (!wglChoosePixelFormatARB(platform::getDC(), (const int*)attr, NULL, 1, &pixelFormat, &count)) {
					cout << "No suitable pikksel formattii" << endl;
					return;
				}
			}
			else if (format == rgb16f) {
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
					console::print(1.0f, 0.0f, 0.0f, "Could not find suitable pbuffer pixel format!");
					return;
				}

				pAttrib[1] = WGL_TEXTURE_RGB_ARB;
			}

			if (count == 0) {
				console::print(1.0f, 0.0f, 0.0f, "Could not find suitable pbuffer pixel format!");
				return;
			}

			pb = wglCreatePbufferARB(platform::getDC(), pixelFormat, width, height, pAttrib);

			if (!pb) {
				console::print(1.0f, 0.0f, 0.0f, "Could not create pbuffer handle!");
				return;
			}

			dc = wglGetPbufferDCARB(pb);

			if (!dc) {
				console::print(1.0f, 0.0f, 0.0f, "Could not create pbuffer device context!");
				return;
			}
		}

		void PixelBuffer::destroy(void)
		{
			wglReleaseTexImageARB(pb, WGL_FRONT_LEFT_ARB);
		    wglReleasePbufferDCARB(pb, dc);
			wglDestroyPbufferARB(pb);
		}

		void PixelBuffer::enable(void)
		{
			//wglMakeCurrent(dc, platform::getRC()); //wglGetCurrentContext());
			wglMakeContextCurrentARB(dc, dc, platform::getRC());

			glDrawBuffer(GL_FRONT);
			glReadBuffer(GL_FRONT);

			glViewport(0, 0, width, height);
		}

		void PixelBuffer::disable(void)
		{
			//wglMakeCurrent(platform::getDC(), platform::getRC()); //wglGetCurrentDC(), wglGetCurrentContext());
			wglMakeContextCurrentARB(platform::getDC(), platform::getDC(), platform::getRC());

			glDrawBuffer(GL_BACK);
			glReadBuffer(GL_BACK);
			
			platform::forceResize(platform::getScreenWidth(), platform::getScreenHeight());
		}

	}
}
