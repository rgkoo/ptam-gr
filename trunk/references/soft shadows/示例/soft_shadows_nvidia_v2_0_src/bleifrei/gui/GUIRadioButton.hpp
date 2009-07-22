
#ifndef _include_bleifrei_GUIRadioButton_hpp
#define _include_bleifrei_GUIRadioButton_hpp 


// includes
#include "GUIFocusComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUIRadioButton : public GUIFocusComponent {
		public:
			void select(void);

			bool isSelected(void) const					{ return id == selected;	}

			void setId(int id)							{ this->id = id;			}
			int getId(void)								{ return id;				}

		protected:
			GUIRadioButton(int &selected) : id(0), selected(selected) {}

			void onClick(int x, int y);

			void onChange(void);

		private:
			int id;
			int &selected;
		};

	}
}


#endif //_include_bleifrei_GUIRadioButton_hpp
