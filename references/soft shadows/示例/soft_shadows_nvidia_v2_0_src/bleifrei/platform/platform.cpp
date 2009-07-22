/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <iostream>

#include "../gfx/opengl.hpp"
#include "../input/mouse.hpp"

#include "types.hpp"

#include "Timer.hpp"
#include "platform.hpp"

using namespace bleifrei::input;
using namespace std;

#define CLASS_NAME "BleifreiWinClass"

// Function declarations
bool initApplication(void);
void destroyApplication(void);

namespace bleifrei {
	namespace platform {

		void dummy(void)		{}
		void dummy(int)			{}
		void dummy(int, int)	{}
		void dummy(char)		{}

		static platform_callback_void update = dummy;
		static platform_callback_void render = dummy;
		static platform_callback_int specialkey = dummy;
		static platform_callback_int_int resize = dummy;
		static platform_callback_char keyboard = dummy;

		int width;
		int height;
		int bit_depth;
		bool fullscreen;

		float step;

		// Instance handle
		static HINSTANCE hInst;

		// Device context and window handles
		HGLRC hRC;
		HDC hDC;

		static HWND hWnd;

		static bool launchFullscreen(void);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static void setupPixelFormat(void);

		void forceResize(int width, int height)
		{
			resize(width, height);
		}

		void createConsole(void)
		{
			AllocConsole();
			SetConsoleTitle("Debug output");
			freopen("CONIN$","rb",stdin);
			freopen("CONOUT$","wb",stdout);
			freopen("CONOUT$","wb",stderr);
		}

		void destroyConsole(void)
		{
			FreeConsole();
		}

		void setConsoleColor(float r, float g, float b)
		{
			word attrib = (r > 0.5 || g > 0.5 || b > 0.5) ? FOREGROUND_INTENSITY : 0;
			if (r > 0.25f) attrib |= FOREGROUND_RED;
			if (g > 0.25f) attrib |= FOREGROUND_GREEN;
			if (b > 0.25f) attrib |= FOREGROUND_BLUE;

			SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), attrib);
		}

		void setUpdateFunc(platform_callback_void updateFunc)
		{
			update = updateFunc;
		}

		void setRenderFunc(platform_callback_void renderFunc)
		{
			render = renderFunc;
		}

		void setResizeFunc(platform_callback_int_int resizeFunc)
		{
			resize = resizeFunc;
		}

		void setSpecialKeyFunc(platform_callback_int specialkeyFunc)
		{
			specialkey = specialkeyFunc;
		}

		void setKeyboardFunc(platform_callback_char keyboardFunc)
		{
			keyboard = keyboardFunc;
		}

		bool createWindow(const char *application_title, int w, int h, int bpp, bool use_fullscreen)
		{
			WNDCLASSEX windowClass;
			DWORD extendedWindowStyle;
			DWORD windowStyle;
			RECT windowRect;

			width = w;
			height = h;
			bit_depth = bpp;
			fullscreen = use_fullscreen;

			if (fullscreen) fullscreen = launchFullscreen();

			windowRect.top = 0;
			windowRect.left = 0;
			windowRect.bottom = height;
			windowRect.right = width;
			
			windowClass.cbSize			= sizeof(WNDCLASSEX);	
			windowClass.style			= CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc		= WndProc;
			windowClass.cbClsExtra		= 0;
			windowClass.cbWndExtra		= 0;
			windowClass.hInstance		= hInst;
			windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
			windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground	= NULL;
			windowClass.lpszMenuName	= NULL;
			windowClass.lpszClassName	= CLASS_NAME;
			windowClass.hIconSm			= LoadIcon(NULL,IDI_APPLICATION);

			if (!RegisterClassEx(&windowClass)) return false;

			if (fullscreen)	{
				extendedWindowStyle =	WS_EX_APPWINDOW;
				windowStyle = WS_POPUP;
				ShowCursor(false);
			}
			else {
				extendedWindowStyle = NULL;
				windowStyle =	WS_OVERLAPPEDWINDOW |
								WS_VISIBLE |
								WS_SYSMENU |
								WS_CLIPCHILDREN |
								WS_CLIPSIBLINGS;

				AdjustWindowRectEx(&windowRect, windowStyle, false, extendedWindowStyle);
				windowRect.bottom -= windowRect.top;
				windowRect.right -= windowRect.left;
				windowRect.left = 0;
				windowRect.top = 0;
			}

			hWnd = CreateWindowEx(extendedWindowStyle,
							CLASS_NAME,
							application_title,
							windowStyle,
							windowRect.left, windowRect.top,
							windowRect.right, windowRect.bottom,
							NULL,
							NULL,
							hInst,
							NULL);

			if (!hWnd) return false;
			
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);

			hDC = GetDC(hWnd);
			setupPixelFormat();

			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);

			return true;
		}

		void destroyWindow(void)
		{
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);
			if (fullscreen) ChangeDisplaySettings(NULL, 0);

			// TEST
			DestroyWindow(hWnd);
			UnregisterClass(CLASS_NAME, hInst);
		}

		static bool shutdown;

		void quit(void)
		{
			shutdown = true;
		}

		// Internal functions

		static bool launchFullscreen(void)
		{
			DEVMODE devModeScreen;
			memset(&devModeScreen, 0, sizeof(devModeScreen));
			devModeScreen.dmSize = sizeof(devModeScreen);
			devModeScreen.dmPelsWidth = width;
			devModeScreen.dmPelsHeight = height;
			devModeScreen.dmBitsPerPel = bit_depth;
			devModeScreen.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			return (ChangeDisplaySettings(&devModeScreen, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
		}

		static void setupPixelFormat(void)
		{
			int nPixelFormat;

			PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW |
					PFD_SUPPORT_OPENGL |
					PFD_DOUBLEBUFFER,
					PFD_TYPE_RGBA,
					bit_depth,
					0,0,0,0,0,0,
					0,
					0,
					0,
					0,0,0,0,
					32,
					8,
					0,
					PFD_MAIN_PLANE,
					0,
					0,0,0 };

			nPixelFormat = ChoosePixelFormat(hDC, &pfd);

			SetPixelFormat(hDC, nPixelFormat, &pfd);
		}



		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch(message) {

/*			case WM_MOUSEWHEEL:
				mouse_z = GET_WHEEL_DELTA_WPARAM(wParam);
				break;
*/
			case WM_KEYDOWN:
				specialkey((int)wParam);
				break;

			case WM_CHAR:
				keyboard((char)wParam);
				break;

			case WM_CREATE:
				break;

			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
				break;

			case WM_SIZE:
				width = LOWORD(lParam);
				height = HIWORD(lParam);
				resize(width, height);
				break;

			default:
				break;
			}

			return (DefWindowProc(hwnd, message, wParam, lParam));
		}

	}
}

using namespace bleifrei::platform;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;
	ShowCursor(false);

	hInst = hInstance;

	if (!initApplication()) {
	}
	else if (!hWnd) {
	}
	else {
		resize(width, height);

		shutdown = false;
		Timer timer;

		while (!shutdown) {									// Enter main event loop
			if (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE)) {
				if (msg.message == WM_QUIT)	{
					shutdown = true;
				}
				else {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			step = timer.getTimeAndReset() * 0.001f;	// Calculate elapsed frametime

			mouse::update();

			update();									// Temporarily transfer control to application
			render();

			glFlush();									// Swap double buffers
			SwapBuffers(hDC);
		}
	}

	destroyApplication();

	ShowCursor(true);
	return (int)msg.wParam;
}
