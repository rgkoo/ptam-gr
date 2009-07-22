
#ifndef _include_ScrollBar_hpp_
#define _include_ScrollBar_hpp_


// includes
#include "bleifrei/gui/GUIScrollBar.hpp"

class ScrollBar : public bleifrei::gui::GUIScrollBar {
public:
	ScrollBar(int x, int y, int width, int height);
	~ScrollBar(void);
protected:
	void onPaintOverlay(int absx, int absy);
};


#endif //_include_ScrollBar_hpp_
