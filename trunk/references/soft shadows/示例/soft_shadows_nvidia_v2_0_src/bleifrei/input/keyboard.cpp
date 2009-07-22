
// includes
#include "../gfx/opengl.hpp"

namespace bleifrei {
	namespace input {
		namespace keyboard {

			bool isDown(int key)
			{
				return (GetAsyncKeyState(key) & 0x8000) != 0;
			}

		}
	}
}
