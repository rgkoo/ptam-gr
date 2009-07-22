
#ifndef _include_CheckBox_hpp_
#define _include_CheckBox_hpp_ 


// includes
#include <string>

#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUICheckBox.hpp"

class CheckBox : public bleifrei::gui::GUICheckBox {
public:
	CheckBox(const std::string &text, bleifrei::gfx::Font *font, int x, int y, int width, int height);

protected:
	void onPaintOverlay(int absx, int absy);

protected:
	std::string text;
	bleifrei::gfx::Font *font;
};


#endif //_include_CheckBox_hpp_

