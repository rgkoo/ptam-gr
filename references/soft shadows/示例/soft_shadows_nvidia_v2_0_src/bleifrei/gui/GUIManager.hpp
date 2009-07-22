
#ifndef _include_bleifrei_GUIManager_hpp_
#define _include_bleifrei_GUIManager_hpp_ 


// includes
#include "GUIContainer.hpp"

namespace bleifrei {
	namespace gui {

		class GUIManager {
		public:
			GUIManager(void);
			~GUIManager(void);

			void render(void);
			void update(void);
			void resize(int width, int height);
			void keyTyped(unsigned char ch);
			void keyDown(int key);

			void add(GUIComponent *component)		{ container.add(component);		}
		protected:
			GUIContainer container;

			int mouse_x;
			int mouse_y;
			int mouse_old_x;
			int mouse_old_y;

			bool mouse_button[3];

			int width;
			int height;
		};

	}
}

#endif //_include_GUIManager_hpp_ 