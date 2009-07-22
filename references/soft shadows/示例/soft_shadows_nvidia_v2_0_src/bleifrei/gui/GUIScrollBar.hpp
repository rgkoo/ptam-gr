
#ifndef _include_bleifrei_GUIScrollBar_hpp_
#define _include_bleifrei_GUIScrollBar_hpp_ 


// includes
#include "GUIFocusComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUIScrollBar : public GUIFocusComponent {
		public:
			void setMax(int max);
			void setMin(int min);
			void setValue(int value);

			int getMax(void) const					{ return this->max;		}
			int getMin(void) const					{ return this->min;		}
			int getValue(void) const				{ return this->value;	}

			const int *getValuePointer(void) const	{ return &value;		}
		protected:
			GUIScrollBar(void)
			{
				max = 1000;
				min = value = 0;
				position = 0;
				slider_size = 0;
			}

			bool isVertical(void) const				{ return vertical;		}

			void onMouseDown(int x, int y);
			void onMouseMove(int x, int y);
			void onResize(void);
			void getValueFromPosition(int x, int y);
			void setSliderSize(int size)			{ slider_size = size;	}

			int getSliderSize(void)					{ return slider_size;	}

			float getPosition(void)					{ return position;		}

			virtual void onChange(void);
		private:
			bool vertical;
			int min, max;
			int value;
			int slider_size;
			float position;
		};
	}
}


#endif //_include_GUIScrollBar_hpp_
