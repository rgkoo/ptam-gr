
#ifndef _include_Frame_hpp_
#define _include_Frame_hpp_ 


// includes
#include "bleifrei/gui/GUIContainer.hpp"

class Frame : public bleifrei::gui::GUIContainer {
public:
	Frame(int x, int y, int width, int height);
protected:
	void onPaintBackground(int absx, int absy);
	void onPaintOverlay(int absx, int absy);
};


#endif //_include_Frame_hpp_
