
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "Frame.hpp"

Frame::Frame(int x, int y, int width, int height)
{
	move(x, y);
	resize(width, height);
}

void Frame::onPaintBackground(int absx, int absy)
{
	glColor4f(0, 0, 0, 0.5f);
	glBegin(GL_QUADS);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();
}

void Frame::onPaintOverlay(int absx, int absy)
{
	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINE_LOOP);
		glVertex2i(absx, absy);
		glVertex2i(absx, absy + getHeight());
		glVertex2i(absx + getWidth(), absy + getHeight());
		glVertex2i(absx + getWidth(), absy);
	glEnd();
}
