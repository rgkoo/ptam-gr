
#ifndef _include_bleifrei_RadioButton_hpp_
#define _include_bleifrei_RadioButton_hpp_ 


// includes
#include <string>

#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUIRadioButton.hpp"

class RadioButton : public bleifrei::gui::GUIRadioButton {
public:
	RadioButton(int id, int &selected, const std::string &text, bleifrei::gfx::Font *font, int x, int y, int width, int height);

protected:
	void onPaintOverlay(int absx, int absy);

protected:
	std::string text;
	bleifrei::gfx::Font *font;
};


#endif //_include_bleifrei_RadioButton_hpp_
