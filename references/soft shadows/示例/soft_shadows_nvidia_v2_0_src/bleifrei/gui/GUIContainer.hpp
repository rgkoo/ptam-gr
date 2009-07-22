
#ifndef _include_bleifrei_GUIContainer_hpp_
#define _include_bleifrei_GUIContainer_hpp_ 


// includes
#include "GUIFocusComponent.hpp"

namespace bleifrei {
	namespace gui {

		class GUIContainer : public GUIFocusComponent {
		public:
			GUIContainer(void) : GUIFocusComponent()			{}

			void add(GUIComponent *component)					{ children.push_back(component);		}
		};

	}
}


#endif //_include_bleifrei_GUIContainer_hpp_
