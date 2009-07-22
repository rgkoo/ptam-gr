
// includes
#include "GUIRadioButton.hpp"

namespace bleifrei {
	namespace gui {

		void GUIRadioButton::select(void)
		{
			if (!isSelected()) {
				selected = id;
				onChange();
			}
		}

		void GUIRadioButton::onClick(int x, int y)
		{
			select();
		}

		void GUIRadioButton::onChange(void)
		{
		}

	}
}