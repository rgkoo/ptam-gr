
#ifndef _include_bleifrei_keyboard_hpp_
#define _include_bleifrei_keyboard_hpp_ 


// includes
#include "../gfx/opengl.hpp"

namespace bleifrei {
	namespace input {
		namespace keyboard {

			enum Key {
				Escape = VK_ESCAPE,
				Pause = VK_PAUSE,
				Left = VK_LEFT,
				Right = VK_RIGHT,
				Up = VK_UP,
				Down = VK_DOWN,
				Space = VK_SPACE,
				LeftShift = VK_LSHIFT,
				LeftControl = VK_LCONTROL
			};

			bool isDown(int key);

		}
	}
}


#endif //_include_bleifrei_keyboard_hpp_ 
