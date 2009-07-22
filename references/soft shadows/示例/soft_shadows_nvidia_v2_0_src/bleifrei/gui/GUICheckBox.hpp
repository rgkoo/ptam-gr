
#ifndef _include_bleifrei_GUICheckBox_hpp_
#define _include_bleifrei_GUICheckBox_hpp_ 

// includes
#include <string>

#include "GUIFocusComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUICheckBox : public GUIFocusComponent {
		public:
			void setChecked(bool checked)				{ this->checked = checked;	}
			bool isChecked(void) const					{ return checked;			}

		protected:
			GUICheckBox(void) : checked(false)			{}

			void onClick(int x, int y);
		private:
			bool checked;
		};

	}
}

#endif //_include_GUICheckBox_hpp_