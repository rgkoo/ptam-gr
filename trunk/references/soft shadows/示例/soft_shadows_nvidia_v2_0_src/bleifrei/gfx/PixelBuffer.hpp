/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_PixelBuffer_hpp_
#define _include_bleifrei_PixelBuffer_hpp_

// includes
#include "../gfx/opengl.hpp"

namespace bleifrei {
	namespace gfx {

		class PixelBuffer {
		public:
			enum Format {
				rgba8,
				rgb16f
			};

			PixelBuffer(void) {}
			PixelBuffer(const PixelBuffer &p);
			PixelBuffer(int width, int height, Format format = rgba8);

			void destroy(void);

			void enable(void);
			void disable(void);

			void bind(void)							{ wglBindTexImageARB(pb, WGL_FRONT_LEFT_ARB);		}
			void release(void)						{ wglReleaseTexImageARB(pb, WGL_FRONT_LEFT_ARB);	}
		protected:
			int width, height;

			HPBUFFERARB pb;							// Handle
			HDC dc;									// Device context
		};

	}
}

#endif //_include_bleifrei_PixelBuffer_hpp_