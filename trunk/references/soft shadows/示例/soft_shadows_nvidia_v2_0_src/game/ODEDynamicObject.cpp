
// includes
#include "bleifrei/math/Matrix_op.hpp"

#include "ODEDynamicObject.hpp"

using namespace bleifrei::math;

const float disable_linear_threshold = 0.08f;
const float disable_angular_threshold = 0.08f;
const int disable_steps_threshold = 10;

void ODEDynamicObject::update(void)
{
	const Vector4 &rotation = *(Vector4 *)dBodyGetQuaternion(body);
	const Vector3 &position = *(Vector3 *)dBodyGetPosition(body);

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);
	glRotatef(360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);

	glLoadIdentity();

	glRotatef(-360.0f * acosf(rotation[0]) / M_PI, rotation[1], rotation[2], rotation[3]);
	glTranslatef(-position[0], -position[1], -position[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_inverse[0]);

	if (dBodyIsEnabled(body)) {
		const Vector3 &linear_velocity = *(Vector3 *)dBodyGetLinearVel(body);

		if (linear_velocity * linear_velocity > disable_linear_threshold) {
			disable_steps = 0;

			return;
		}

		const Vector3 &angular_velocity = *(Vector3 *)dBodyGetAngularVel(body);

		if (angular_velocity * angular_velocity > disable_angular_threshold) {
			disable_steps = 0;

			return;
		}

		if (++disable_steps > disable_steps_threshold) dBodyDisable(body);
	}	
}
