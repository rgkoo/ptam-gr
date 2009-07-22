
#ifndef _include_TextBox_hpp_
#define _include_TextBox_hpp_ 


// includes
#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUITextBox.hpp"

class TextBox : public bleifrei::gui::GUITextBox {
public:
	TextBox(bleifrei::gfx::Font *font, int x, int y, int width, int height);

protected:
	void onPaintOverlay(int absx, int absy);
	void onChange(void);

protected:
	bleifrei::gfx::Font *font;

	unsigned int first;
	unsigned int visible;
};


#endif //_include_TextBox_hpp_
