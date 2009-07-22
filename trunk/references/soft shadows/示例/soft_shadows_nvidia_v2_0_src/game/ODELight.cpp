
// includes
#include "bleifrei/gfx/opengl.hpp"

#include "ODELight.hpp"

using namespace bleifrei::math;
using namespace bleifrei::tex;

void ODESphericalLight::render(void)
{
	glPushMatrix();
		glMultMatrixf(&matrix[0]);
		model.render();
	glPopMatrix();
}

void ODESphericalLight::update(void)
{
	const Vector4 &rotation = *(Vector4 *)dBodyGetQuaternion(body);
	assign(position, *(Vector3 *)dBodyGetPosition(body));

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);
	glRotatef(360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
}

void ODERectangularLight::render(void)
{
	glPushMatrix();
		glMultMatrixf(&matrix[0]);
		glTranslatef(0, height * 0.5f, 0);
		model.render();
	glPopMatrix();
}

void ODERectangularLight::update(void)
{
	float fulmatris[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	const Vector4 &rotation = *(Vector4 *)dBodyGetQuaternion(body);
	assign(position, *(Vector3 *)dBodyGetPosition(body));

	glLoadIdentity();

	glMultMatrixf(fulmatris);
	glTranslatef(0, height * 0.5f, 0);
	glRotatef(-360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(-position[0], -position[1], -position[2]);	
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_rot_inverse[0]);

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);
	glRotatef(360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(0, -height * 0.5f, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);

	position = matrix.getColumn(3);
}

void ODETexturedLight::render(void)
{
	glPushMatrix();
		glMultMatrixf(&matrix[0]);
		glTranslatef(0, height * 0.5f, 0);
		model.render();
	glPopMatrix();
}

void ODETexturedLight::update(void)
{
	float fulmatris[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	const Vector4 &rotation = *(Vector4 *)dBodyGetQuaternion(body);
	assign(position, *(Vector3 *)dBodyGetPosition(body));

	glLoadIdentity();

	glMultMatrixf(fulmatris);
	glTranslatef(0, height * 0.5f, 0);
	glRotatef(-360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(-position[0], -position[1], -position[2]);	
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_rot_inverse[0]);

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);
	glRotatef(360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(0, -height * 0.5f, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);

	position = matrix.getColumn(3);
}