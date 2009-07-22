
#ifndef _include_Label_hpp_
#define _include_Label_hpp_ 


// includes
#include <string>

#include "bleifrei/gfx/Font.hpp"
#include "bleifrei/gui/GUIFocusComponent.hpp"

class Label : public bleifrei::gui::GUIFocusComponent {
public:
	Label(const bleifrei::math::Vector3 &color, const std::string &text, bleifrei::gfx::Font *font, int x, int y, int width, int height);

protected:
	void onPaintOverlay(int absx, int absy);

protected:
	std::string text;
	bleifrei::gfx::Font *font;
	bleifrei::math::Vector3 color;
};


#endif //_include_Label_hpp_
