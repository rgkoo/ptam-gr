
// includes
#include "Object.hpp"

using namespace bleifrei::math;
using namespace bleifrei::mesh;

Object::Object(Mesh *mesh, Model model)
	: mesh(mesh), model(model), shadow_mode(SoftShadow)
{
	matrix.identity();
	matrix_inverse.identity();
}

Object::~Object(void)
{
}

void Object::render(void)
{
	glPushMatrix();
		glMultMatrixf(&matrix[0]);
		model.render();
	glPopMatrix();
}

void Object::update(void)
{
}
