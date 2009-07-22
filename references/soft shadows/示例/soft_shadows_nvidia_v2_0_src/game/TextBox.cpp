
// includes
#include "bleifrei/tex/Texture.hpp"

#include "TextBox.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::tex;

const int space = 2;

TextBox::TextBox(Font *font, int x, int y, int width, int height)
	: font(font), visible(0), first(0)
{
	move(x, y);
	resize(width, height);
}

void TextBox::onPaintOverlay(int absx, int absy)
{
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

	if (getHeight() >= font->getHeight() + 2 * space) {
		glColor4f(1,1,1,1);
		Texture2::enable();
		font->bindTexture();
		int ypos = absy + (getHeight() - font->getHeight()) / 2;
		font->print(absx + space, ypos, 1.0f, text.substr(first, visible));
		if (hasFocus()) font->print(absx + space + font->getWidth(text.substr(first, cursorpos - first)), ypos, 1.0f, "|");
		Texture2::disable();
	}
}

void TextBox::onChange(void)
{
	if (cursorpos > text.length()) cursorpos = (unsigned int)text.length();
	while (first > cursorpos) {
		first -= 4;
	}
	if (first < 0) first = 0;

	visible = font->getMaxChars(&(text.c_str()[first]), (unsigned int)text.length() - first, getWidth() - 2 * space);

	while (cursorpos - first > visible) {
		first += visible / 4 + 1;
		if (first > cursorpos) {
			first = cursorpos;
		}
		visible = font->getMaxChars(&(text.c_str()[first]), (unsigned int)text.length() - first, getWidth() - 2 * space);
	}
}
