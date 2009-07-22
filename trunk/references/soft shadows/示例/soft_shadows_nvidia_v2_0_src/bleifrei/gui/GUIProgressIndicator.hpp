
#ifndef _include_bleifrei_GUIProgressIndicator_hpp_
#define _include_bleifrei_GUIProgressIndicator_hpp_ 


// includes
#include "GUIComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUIProgressIndicator : public GUIComponent {
		public:
			void setMax(int max);
			void setMin(int min);
			void setValue(int value);

			int getMax(void) const						{ return this->max;		}
			int getMin(void) const						{ return this->min;		}
			int getValue(void) const					{ return this->value;	}

		protected:
			GUIProgressIndicator(void)
			{
				value = min = 0;
				max = 1000;
				position = 0;
			}

			bool isVertical(void)						{ return vertical;		}

			float getPosition(void)						{ return position;		}
			void onResize(void);
		protected:
			bool vertical;
			int min, max;
			int value;
			float position;
		};
	}
}

#endif //_include_GUIProgressIndicator_hpp
