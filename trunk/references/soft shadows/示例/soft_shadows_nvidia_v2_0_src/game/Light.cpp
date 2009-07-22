
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "Light.hpp"

using namespace bleifrei::math;
using namespace bleifrei::tex;

Light::Light(const Vector3 &position, Type type)
	: position(position[0], position[1], position[2], 1), type(type)
{
	glLoadIdentity();
	glTranslatef(position[0], position[1], position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
}

void Light::render(void)
{
}

void Light::update(void)
{
	glLoadIdentity();
	glTranslatef(position[0], position[1], position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
}

RectangularLight::RectangularLight(const Vector3 &position, float width, float length)
	: Light(position, Rectangular), width(width), length(length)
{
}

void RectangularLight::update(void)
{
	float fulmatris[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glLoadIdentity();
	glMultMatrixf(fulmatris);
	glTranslatef(-position[0], -position[1], -position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_rot_inverse[0]);

	Light::update();
}

TexturedLight::TexturedLight(Texture2 texture, Texture2 coverage, const Vector3 &position, float width, float length)
	: RectangularLight(position, width, length), texture(texture), coverage(coverage)
{
	type = Textured;
}
