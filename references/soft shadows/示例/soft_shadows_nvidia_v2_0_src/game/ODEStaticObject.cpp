
// includes
#include "bleifrei/math/Matrix_op.hpp"

#include "ODEStaticObject.hpp"

using namespace bleifrei::math;

ODEStaticObject::ODEStaticObject(bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model)
	: Object(mesh, model)
{
	matrix.identity();
	matrix_inverse.identity();
	geometry_data.setOwner(this);
}

ODEStaticObject::ODEStaticObject(dGeomID geometry, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model)
	: geometry(geometry), Object(mesh, model)
{
	Vector4 rotation;
	const Vector3 &position = *(Vector3 *)dGeomGetPosition(geometry);

	dGeomGetQuaternion(geometry, &rotation[0]);

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);
	glRotatef(360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);

	glLoadIdentity();

	glRotatef(-360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(-position[0], -position[1], -position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_inverse[0]);
	geometry_data.setOwner(this);
	dGeomSetData(geometry, &geometry_data);
}
