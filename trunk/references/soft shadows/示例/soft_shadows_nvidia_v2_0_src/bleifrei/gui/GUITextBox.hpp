
#ifndef _include_GUITextBox_hpp_
#define _include_GUITextBox_hpp_ 


//includes
#include <string>

#include "GUIFocusComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUITextBox : public GUIFocusComponent {
		public:
			void setText(const std::string &text);

			const std::string &getText(void)			{ return text;				}

		protected:
			GUITextBox(void) : text(), cursorpos(0) {}

			bool onKey(unsigned char key, int x, int y);
			bool onSpecial(int key, int x, int y);

			virtual void onChange(void);

		protected:
			std::string text;

			unsigned int cursorpos;
		};

	}
}

#endif //_include_GUITextBox_hpp_
