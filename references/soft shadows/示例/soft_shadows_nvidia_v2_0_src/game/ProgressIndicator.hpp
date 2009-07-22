
#ifndef _include_ProgressIndicator_hpp_
#define _include_ProgressIndicator_hpp_ 


// includes
#include "bleifrei/gui/GUIProgressIndicator.hpp"

class ProgressIndicator : public bleifrei::gui::GUIProgressIndicator {
public:
	ProgressIndicator(int x, int y, int width, int height);
	~ProgressIndicator(void);

protected:
	void onPaintBackground(int absx, int absy);
	void onPaintOverlay(int absx, int absy);
};

#endif //_include_ProgressIndicator_hpp_
