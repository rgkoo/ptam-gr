
// includes
#include "GUIProgressIndicator.hpp"

namespace bleifrei {
	namespace gui {

		void GUIProgressIndicator::setMax(int max)
		{
			this->max = max;
			setValue(getValue());
		}

		void GUIProgressIndicator::setMin(int min)
		{
			this->min = min;
			setValue(getValue());
		}

		void GUIProgressIndicator::setValue(int value)
		{
			if (value > max) {
				this->value = max;
			}
			else if (value < min) {
				this->value = min;
			}
			else {
				this->value = value;
			}
			position = (value - min + 1) / (float)(max - min + 1);
		}

		void GUIProgressIndicator::onResize(void) {
			vertical = (getWidth() < getHeight()) ? true : false;
		}

	}
}