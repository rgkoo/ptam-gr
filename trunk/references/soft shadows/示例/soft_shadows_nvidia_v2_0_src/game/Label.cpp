
// includes
#include "bleifrei/gfx/opengl.hpp"
#include "bleifrei/tex/Texture.hpp"

#include "Label.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::math;
using namespace bleifrei::tex;
using namespace std;

Label::Label(const Vector3 &color, const string &text, Font *font, int x, int y, int width, int height)
	: text(text), font(font), color(color)
{
	move(x, y);
	resize(width, height);
}

void Label::onPaintOverlay(int absx, int absy)
{
	glColor3fv(&color[0]);
	Texture2::enable();
	font->bindTexture();
	font->print(absx, absy, 1.0f, text);
	Texture2::disable();
}
