
#ifndef _include_bleifrei_GUIScrollBox_hpp_
#define _include_bleifrei_GUIScrollBox_hpp_ 


// includes
#include "GUIContainer.hpp"
#include "GUIScrollBar.hpp"

class GUIScrollBox : public GUIComponent {
public:
	void resizeScrollArea(int width, int height);

protected:
	GUIScrollBox(void);

private:
	GUIContainer *container;
};


#endif //_include_bleifrei_ScrollBox_hpp_
