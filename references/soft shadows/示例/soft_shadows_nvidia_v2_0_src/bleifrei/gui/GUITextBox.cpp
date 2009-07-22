
#include <iostream>

// includes
#include "../input/keyboard.hpp"

#include "GUITextBox.hpp"

using namespace std;

namespace bleifrei {
	namespace gui {

		void GUITextBox::setText(const string &text)
		{
			this->text = text;
			onChange();
		}

		bool GUITextBox::onKey(unsigned char key, int x, int y) {
			if (hasFocus()) {
				if (key == 127) {
					if (text.length() > cursorpos) text.erase(cursorpos, 1);
				}
				else if (key > 31) {
					text.insert(cursorpos, 1, key);
					cursorpos++;
				}
				else {
					switch (key) {
					case 8 :
						if (cursorpos > 0) {
							text.erase(--cursorpos, 1);
						}
						else return false;
						break;

					default:
						return false;
					}
				}
				onChange();
				return true;
			}
			return false;
		}

		bool GUITextBox::onSpecial(int key, int x, int y) {
			if (hasFocus()) {
				switch (key) {
				case VK_DELETE :
					if (text.size() > cursorpos) text.erase(cursorpos, 1);
					break;
				case VK_LEFT:
					if (cursorpos > 0) {
						cursorpos--;
					}
					break;
				case VK_RIGHT:
					if (cursorpos < text.length()) {
						cursorpos++;
					}
					break;
				case VK_HOME:
					cursorpos = 0;
					break;
				case VK_END:
					cursorpos = (unsigned int)text.length();
					break;
				default:
					return false;
				}
				onChange();
				return true;
			}
			return false;
		}

		void GUITextBox::onChange(void)
		{
		}

	}
}