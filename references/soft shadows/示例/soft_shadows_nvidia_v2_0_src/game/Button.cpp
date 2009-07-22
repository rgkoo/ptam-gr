
// includes
#include "bleifrei/gfx/opengl.hpp"
#include "bleifrei/tex/Texture.hpp"

#include "Button.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::tex;
using namespace std;

Button::Button(const string &text, Font *font, int x, int y, int width, int height)
	: text(text), font(font), callback(0)
{
	move(x, y);
	resize(width, height);
}

void Button::onPaintOverlay(int absx, int absy)
{
	if (isClicked())
		glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
	else if (isMouseOver())
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	else
		glColor4f(0.5f, 1.0f, 0.6f, 0.5f);

	glBegin(GL_QUADS);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();

	if (isClicked())
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	else if (isMouseOver())
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	else
		glColor4f(0.5f, 1.0f, 0.6f, 1.0f);

	glBegin(GL_LINE_LOOP);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();

	glColor4f(1,1,1,1);
	Texture2::enable();
	font->bindTexture();
	font->print(absx + (getWidth() - font->getWidth(text)) / 2, absy + (getHeight() - font->getHeight()) / 2, 1.0f, text);
	Texture2::disable();
}

void Button::onClick(int, int)
{
	if (callback) callback(this);
}
