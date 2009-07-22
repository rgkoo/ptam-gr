
// includes
#include "../gfx/opengl.hpp"
#include "../platform/platform.hpp" 

#include "mouse.hpp"

namespace bleifrei {
	namespace input {
		namespace mouse {

			Mode mouse_mode = Absolute;
			POINT mouse_move;
			POINT mouse_pos;

			void setMode(Mode mode)
			{
				if (mode == Relative) {
					SetCursorPos(platform::getScreenWidth() / 2, platform::getScreenHeight() / 2);
				}
				mouse_mode = mode;
			}

			void getMovement(int &x, int &y)
			{
				x = mouse_move.x;
				y = mouse_move.y;
			}

			int getMovementX(void)
			{
				return mouse_move.x;
			}

			int getMovementY(void)
			{
				return mouse_move.y;
			}

			void getPosition(int &x, int &y)
			{
				x = mouse_pos.x;
				y = mouse_pos.y;
			}

			int getPositionX(void)
			{
				return mouse_pos.x;
			}

			int getPositionY(void)
			{
				return mouse_pos.y;
			}

			static int buttons[] = {
				VK_LBUTTON,
				VK_RBUTTON,
				VK_MBUTTON
			};

			bool isDown(int button)
			{
				return (GetAsyncKeyState(buttons[button]) & 0x8000) != 0;
			}

			void update(void)
			{
				GetCursorPos(&mouse_move);
				mouse_move.x -= platform::getScreenWidth() / 2;
				mouse_move.y -= platform::getScreenHeight() / 2;
				SetCursorPos(platform::getScreenWidth() / 2, platform::getScreenHeight() / 2);

				if (mouse_mode != Relative) {
					mouse_pos.x += mouse_move.x;
					mouse_pos.y += mouse_move.y;
					if (mouse_pos.x < 0)
						mouse_pos.x = 0;
					else if (mouse_pos.x >= platform::getScreenWidth())
						mouse_pos.x = platform::getScreenWidth() - 1;
					if (mouse_pos.y < 0)
						mouse_pos.y = 0;
					else if (mouse_pos.y >= platform::getScreenHeight())
						mouse_pos.y = platform::getScreenHeight() - 1;
				}
			}

		}
	}
}