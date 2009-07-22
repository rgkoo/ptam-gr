
// includes
#include "bleifrei/input/keyboard.hpp"
#include "bleifrei/input/mouse.hpp"
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/platform/platform.hpp"

#include "PlayerObject.hpp"

using namespace bleifrei::input;
using namespace bleifrei::math;
using namespace bleifrei;

static const float turn_speed = 2.0f;
static const float mouse_turn_speed = 0.5f;
static const float object_force = 100.0f;
static const float object_jumpforce = 250.0f;
static const float object_torque = 500.0f;
static const float maximum_angular_velocity = 20.0f;

PlayerObject::PlayerObject(dGeomID geometry, dBodyID body, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model)
	: geometry(geometry), body(body), Object(mesh, model), direction(3), on_ground(false), jumping(false)
{
	data.setType(ODEGeomData::PlayerFeet);
	data.setOwner(this);
	dGeomSetData(geometry, &data);
}

void PlayerObject::update(void)
{
	if (keyboard::isDown(keyboard::Right)) direction += turn_speed * platform::getFrameStep();
	if (keyboard::isDown(keyboard::Left)) direction -= turn_speed * platform::getFrameStep();

	direction += mouse::getMovementX() * mouse_turn_speed * platform::getFrameStep();

	if (GetAsyncKeyState('A') & 0x8000) {
		dBodyAddForce(body,
			-sinf(direction) * object_force * platform::getFrameStep(),
			0,
			cosf(direction) * object_force * platform::getFrameStep());
		dBodyAddTorque(body,
			cosf(direction) * object_torque * platform::getFrameStep(),
			0,
			sinf(direction) * object_torque * platform::getFrameStep());

	}
	if (GetAsyncKeyState('D') & 0x8000) {
		dBodyAddForce(body,
			sinf(direction) * object_force * platform::getFrameStep(),
			0,
			-cosf(direction) * object_force * platform::getFrameStep());
		dBodyAddTorque(body,
			-cosf(direction) * object_torque * platform::getFrameStep(),
			0,
			-sinf(direction) * object_torque * platform::getFrameStep());
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		dBodyAddForce(body,
			-cosf(direction) * object_force * platform::getFrameStep(),
			0,
			-sinf(direction) * object_force * platform::getFrameStep());
		dBodyAddTorque(body,
			-sinf(direction) * object_torque * platform::getFrameStep(),
			0,
			cosf(direction) * object_torque * platform::getFrameStep());
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		dBodyAddForce(body,
			cosf(direction) * object_force * platform::getFrameStep(),
			0,
			sinf(direction) * object_force * platform::getFrameStep());
		dBodyAddTorque(body,
			sinf(direction) * object_torque * platform::getFrameStep(),
			0,
			-cosf(direction) * object_torque * platform::getFrameStep());
	}
	if (jumping) {
		if (on_ground) jumping = (GetAsyncKeyState(VK_SPACE) & 0x8000) > 0;
	}
	else if (on_ground && (GetAsyncKeyState(VK_SPACE) & 0x8000)) {
		dBodyAddForce(body,
			surface_normal[0] * object_jumpforce,
			surface_normal[1] * object_jumpforce,
			surface_normal[2] * object_jumpforce);
		jumping = true;
	}
	on_ground = false;

	Vector3 angular_velocity = *(Vector3 *)dBodyGetAngularVel(body);

	if (angular_velocity * angular_velocity > maximum_angular_velocity * maximum_angular_velocity) {
		normalize(angular_velocity);
		dBodySetAngularVel(body, angular_velocity[0] * maximum_angular_velocity, angular_velocity[1] * maximum_angular_velocity, angular_velocity[2] * maximum_angular_velocity);
	}

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
}

void PlayerObject::setOnGround(const Vector3 &normal)
{
	surface_normal = normal;
	on_ground = true;
}
