
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "ProgressIndicator.hpp"

ProgressIndicator::ProgressIndicator(int x, int y, int width, int height)
{
	move(x, y);
	resize(width, height);
}

ProgressIndicator::~ProgressIndicator(void)
{
}

void ProgressIndicator::onPaintBackground(int absx, int absy)
{
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

	glBegin(GL_QUADS);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();
}

void ProgressIndicator::onPaintOverlay(int absx, int absy)
{
	const int space = 3;
	int xpos = (int)((isVertical() ? 1 : position) * (getWidth() - 2 * space));
	int ypos = (int)((isVertical() ? position : 1) * (getHeight() - 2 * space));

	glColor4f(0.0f, 0.0f, position, 0.5f);
	glBegin(GL_QUADS);
		glVertex2i(absx + space, absy - space + getHeight() - ypos);
		glVertex2i(absx + space, absy - space + getHeight());
		glVertex2i(absx + space + xpos, absy - space + getHeight());
		glVertex2i(absx + space + xpos, absy - space + getHeight() - ypos);
	glEnd();

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();

/*	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2i(absx + space, absy - space + getHeight() - ypos);
		glVertex2i(absx + space, absy - space + getHeight());
		glVertex2i(absx + space + xpos, absy - space + getHeight());
		glVertex2i(absx + space + xpos, absy - space + getHeight() - ypos);
	glEnd();*/
}
