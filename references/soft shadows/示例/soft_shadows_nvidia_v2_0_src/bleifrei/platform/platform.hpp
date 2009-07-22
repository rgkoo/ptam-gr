/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */


#ifndef _include_bleifrei_platform_hpp_
#define _include_bleifrei_platform_hpp_

#include "../gfx/opengl.hpp"

namespace bleifrei {
	namespace platform {

		extern int width;
		extern int height;
		extern int bit_depth;
		extern bool fullscreen;

		extern float step;

		extern HGLRC hRC;
		extern HDC hDC;

		inline HGLRC getRC(void)							{ return hRC;			}
		inline HDC getDC(void)								{ return hDC;			}

		inline int getScreenWidth(void)						{ return width;			}
		inline int getScreenHeight(void)					{ return height;		}
		inline int getScreenBpp(void)						{ return bit_depth;		}
		inline bool getFullscreenFlag(void)					{ return fullscreen;	}

		inline float getFrameStep(void)						{ return step;			}

		typedef void (*platform_callback_void)(void);
		typedef void (*platform_callback_int)(int);
		typedef void (*platform_callback_int_int)(int, int);
		typedef void (*platform_callback_char)(char);

		void setUpdateFunc(platform_callback_void);
		void setRenderFunc(platform_callback_void);
		void setResizeFunc(platform_callback_int_int);
		void setSpecialKeyFunc(platform_callback_int);
		void setKeyboardFunc(platform_callback_char);

		void forceResize(int width, int height);

		bool createWindow(const char *title = "bleifrei", int width = 640, int height = 480, int bpp = 32, bool fullscreen = false);
		void destroyWindow(void);

		void quit(void);

		void createConsole(void);
		void destroyConsole(void);
		void setConsoleColor(float r, float g, float b);

	}
}

#endif //_include_bleifrei_platform_hpp_