
#ifndef _include_gfx_RenderTarget_hpp_
#define _include_gfx_RenderTarget_hpp_ 


// includes
#include "opengl.hpp"

namespace bleifrei {
	namespace gfx {

		class RenderTarget {
		public:
			enum Format {
				rgb8,
				rgba8,
				rgba8rect,
				rgba8db,
				rgba8dbrect,
				rgb16f,
				rgb16frect
			};
			enum Buffer {
				Front = WGL_FRONT_LEFT_ARB,
				Back = WGL_BACK_LEFT_ARB
			};
			RenderTarget(int width, int height, Format format = rgba8);
			~RenderTarget(void);

			void bind(Buffer b = Front)				{ wglBindTexImageARB(pb, b);		}
			void release(Buffer b = Front)			{ wglReleaseTexImageARB(pb, b);		}

			void enableOrtho(void);

			static void setActive(RenderTarget *target);
			static void enableOrtho(int width, int height);
			static void disableOrtho(void);
		protected:
			void activate(void);

			int width, height;
			HPBUFFERARB pb;
			HDC dc;
			HGLRC rc;

			static RenderTarget *active;
		};

	}
}


#endif //_include_gfx_RenderTarget_hpp_
