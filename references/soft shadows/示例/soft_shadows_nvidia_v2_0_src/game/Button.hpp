
#ifndef _include_Button_hpp_
#define _include_Button_hpp_ 


// includes
#include <string>

#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUIFocusComponent.hpp"

class Button : public bleifrei::gui::GUIFocusComponent {
public:
	Button(const std::string &text, bleifrei::gfx::Font *font, int x, int y, int width, int height);

	void setCallback(void (*callback)(Button *))								{ this->callback = callback;	}

protected:
	void onPaintOverlay(int absx, int absy);
	void onClick(int x, int y);

protected:
	std::string text;
	bleifrei::gfx::Font *font;

	void (*callback)(Button *);
};


#endif //_include_Button_hpp_
