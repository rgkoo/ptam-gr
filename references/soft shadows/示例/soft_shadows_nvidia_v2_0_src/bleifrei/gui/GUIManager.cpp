
// includes
#include "../input/mouse.hpp"
#include "../platform/platform.hpp"

#include "GUIManager.hpp"

using namespace bleifrei::input;
using namespace bleifrei;

namespace bleifrei {
	namespace gui {

		GUIManager::GUIManager(void)
		{
			resize(platform::getScreenWidth(), platform::getScreenHeight());
			mouse_button[0] = mouse_button[1] = mouse_button[2] = 0;
		}

		GUIManager::~GUIManager(void)
		{
			container.cleanup();
		}

		void GUIManager::render(void)
		{
			container.paint(0, 0);
		}

		void GUIManager::update(void)
		{
			mouse::getPosition(mouse_x, mouse_y);

			for (int button = 0; button < 3; ++button)
			{
				if (mouse::isDown(button)) {
					if (!mouse_button[button]) {
						mouse_button[button] = true;
						container.mouseDown(button, mouse_x, mouse_y);
					}
				}
				else {
					if (mouse_button[button]) {
						mouse_button[button] = false;
						container.mouseUp(button, mouse_x, mouse_y);
					}
				}
			}

			if (mouse_x != mouse_old_x || mouse_y != mouse_old_y)
			{
				if (mouse_button[0] || mouse_button[1] || mouse_button[2]) {
					container.motion(mouse_x, mouse_y);
				}
				else {
					container.passiveMotion(mouse_x, mouse_y);
				}
			}

			mouse_old_x = mouse_x;
			mouse_old_y = mouse_y;

			container.update();
		}

		void GUIManager::resize(int width, int height)
		{
			container.resize(width, height);
		}

		void GUIManager::keyTyped(unsigned char ch)
		{
			container.key(ch, mouse_x, mouse_y);
		}

		void GUIManager::keyDown(int key)
		{
			container.special(key, mouse_x, mouse_y);
		}

	}
}