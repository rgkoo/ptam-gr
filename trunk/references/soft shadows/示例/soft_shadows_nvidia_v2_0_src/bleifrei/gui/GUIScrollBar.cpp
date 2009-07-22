
// includes
#include "GUIScrollBar.hpp"

namespace bleifrei {
	namespace gui {

		void GUIScrollBar::setMax(int max) {
			this->max = max;
			setValue(getValue());
		}

		void GUIScrollBar::setMin(int min) {
			this->min = min;
			setValue(getValue());
		}

		void GUIScrollBar::setValue(int value) {
			if (value > max) {
				this->value = max;
			}
			else if (value < min) {
				this->value = min;
			}
			else {
				this->value = value;
			}
			position = (this->value - min) / (float)(max - min);
			this->onChange();
		}

		void GUIScrollBar::onMouseDown(int x, int y) {
			getValueFromPosition(x, y);
		}

		void GUIScrollBar::onMouseMove(int x, int y) {
			if (isClicked()) getValueFromPosition(x, y);
		}

		void GUIScrollBar::onResize() {
			vertical = (getWidth() > getHeight()) ? false : true;
		}

		void GUIScrollBar::onChange() {
		}

		void GUIScrollBar::getValueFromPosition(int x, int y) {
			if (getMax() > getMin()) {
				if (vertical) {
					int p = y - getSliderSize() / 2 - 1;
					setValue(getMin() + p * (getMax() - getMin() + 1) / (getHeight() - getSliderSize()));
				}
				else {
					int p = x - getSliderSize() / 2 - 1;
					setValue(getMin() + p * (getMax() - getMin() + 1) / (getWidth() - getSliderSize()));
				}
			}
			else {
				setValue(getMin());
			}
		}

	}
}
