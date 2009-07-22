
#ifndef _include_bleifrei_GUIFocusComponent_hpp_ 
#define _include_bleifrei_GUIFocusComponent_hpp_ 


// includes
#include "GUIComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUIFocusComponent : public GUIComponent {
		public:
			GUIFocusComponent(void)		{ setWantFocus(true);		}
		};

	}
}

#endif //_include_GUIFocusComponent_hpp_ 
