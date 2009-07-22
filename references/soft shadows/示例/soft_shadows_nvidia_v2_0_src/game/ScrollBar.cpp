
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "ScrollBar.hpp"


ScrollBar::ScrollBar(int x, int y, int width, int height)
{
	move(x, y);
	resize(width, height);
	setSliderSize(16);
}

ScrollBar::~ScrollBar(void)
{
}

void ScrollBar::onPaintOverlay(int absx, int absy)
{
	const int space = 3;

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

	if (isVertical()) {
		int pos = (int)(getPosition() * (getHeight() - getSliderSize()));
		glBegin(GL_LINE_LOOP);
			glVertex2i(absx + space, absy + space + pos);
			glVertex2i(absx + space, absy - space + pos + getSliderSize());
			glVertex2i(absx - space + getWidth(), absy - space + pos + getSliderSize());
			glVertex2i(absx - space + getWidth(), absy + space + pos);
		glEnd();
	}
	else {
		int pos = (int)(getPosition() * (getWidth() - getSliderSize()));
		glBegin(GL_LINE_LOOP);
			glVertex2i(absx + space + pos, absy + space);
			glVertex2i(absx + space + pos, absy - space + getHeight());
			glVertex2i(absx - space + pos + getSliderSize(), absy - space + getHeight());
			glVertex2i(absx - space + pos + getSliderSize(), absy + space);
		glEnd();
	}

	glBegin(GL_LINE_LOOP);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();
}
