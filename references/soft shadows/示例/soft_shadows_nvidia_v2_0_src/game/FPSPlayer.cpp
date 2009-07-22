
// includes
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/input/keyboard.hpp"
#include "bleifrei/input/mouse.hpp"
#include "bleifrei/platform/platform.hpp"

#include "FPSPlayer.hpp"
#include "TractorBeam.hpp"

using namespace bleifrei::math;
using namespace bleifrei::input;
using namespace bleifrei::tex;
using namespace bleifrei;

const float head_position = 3.0f;
const float head_radius = 1.0f;
const float head_mass = 1.0f;

const float feet_radius = 1.0f;
const float feet_mass = 1.0f;

const float mouse_turn_speed = 0.5f * 0.01f;

const float maximum_angular_vel = 80.0f;

bool mouse_invert = true;

const float object_force = -2000.0f;
const float object_torque = 0.0f; //-4500.0f;
const float jump_force = 550.0f;

FPSPlayer::FPSPlayer(const Vector3 &position, dWorldID world, dSpaceID space, TextureManager &texture_manager)
	: direction(0), pitch(0)
{
	dMass mass;

	head_body = dBodyCreate(world);
	dBodySetPosition(head_body, position[0], position[1] + head_position, position[2]);
	dMassSetSphereTotal(&mass, head_mass, head_radius);
	dBodySetMass(head_body, &mass);
	dBodySetGravityMode(head_body, 0);

	head_data.setOwner(this);
	head_data.setType(ODEGeomData::PlayerHead);
	head_geometry = dCreateSphere(space, head_radius);
	dGeomSetBody(head_geometry, head_body);
	dGeomSetData(head_geometry, &head_data);
	

	feet_body = dBodyCreate(world);
	dBodySetPosition(feet_body, position[0], position[1], position[2]);
	dMassSetSphereTotal(&mass, feet_mass, feet_radius);
	dBodySetMass(feet_body, &mass);

	feet_data.setOwner(this);
	feet_data.setType(ODEGeomData::PlayerFeet);
	feet_geometry = dCreateSphere(space, feet_radius);
	dGeomSetBody(feet_geometry, feet_body);
	dGeomSetData(feet_geometry, &feet_data);

	dJointID joint = dJointCreateBall(world, 0);
	dJointAttach(joint, head_body, feet_body);
	dJointSetBallAnchor(joint, position[0], position[1], position[2]);

	line_of_sight_data.setOwner(this);
	line_of_sight_data.setType(ODEGeomData::LineOfSight);
	line_of_sight = dCreateRay(space, 100);
	dGeomRaySet(line_of_sight,
				position[0], position[1] + head_position, position[2],
				-cosf(direction) * cosf(pitch), -sinf(pitch), -sinf(direction) * cosf(pitch));
	dGeomSetData(line_of_sight, &line_of_sight_data);

	weapon = new TractorBeam(this, texture_manager.load("ray.tga"));
}

FPSPlayer::~FPSPlayer(void)
{
	delete weapon;
}

void FPSPlayer::render(void)
{
}

void FPSPlayer::update(float frame_step)
{
/*	direction += 0.01f * mouse_turn_speed * mouse::getMovementX(); 
	pitch += 0.01f * mouse_turn_speed * mouse::getMovementY() * (mouse_invert ? 1 : -1); 
	if (pitch < -1.5f) {
		pitch = -1.5f;
	}
	else if (pitch > 1.5f) {
		pitch = 1.5f;
	}

	look_direction[0] = cosf(direction) * cosf(pitch);
	look_direction[1] = sinf(pitch);
	look_direction[2] = sinf(direction) * cosf(pitch);
*/
	bool moving = false;
	float force_dir1 = sinf(direction) * frame_step * ground_normal[1];
	float force_dir2 = cosf(direction) * frame_step * ground_normal[1];

	float torque_dir1 = force_dir1 * object_torque * ground_normal[1];
	float torque_dir2 = force_dir2 * object_torque * ground_normal[1];
	force_dir1 *= object_force;
	force_dir2 *= object_force;

	if (keyboard::isDown(keyboard::LeftShift)) {
		force_dir1 *= 3;
		force_dir2 *= 3;
	}

	if (keyboard::isDown('A')) {
		if (on_ground) {
			dBodyAddForce(head_body, -force_dir1, 0, force_dir2);
			dBodyAddForce(feet_body, -force_dir1, 0, force_dir2);
			dBodyAddTorque(feet_body, torque_dir2, 0, torque_dir1);
		}
		moving = true;
	}
	if (keyboard::isDown('D')) {
		if (on_ground) {
			dBodyAddForce(head_body, force_dir1, 0, -force_dir2);
			dBodyAddForce(feet_body, force_dir1, 0, -force_dir2);
			dBodyAddTorque(feet_body, -torque_dir2, 0, -torque_dir1);
		}
		moving = true;
	}
	if (keyboard::isDown('W')) {
		if (on_ground) {
			dBodyAddForce(head_body, -force_dir2, 0, -force_dir1);
			dBodyAddForce(feet_body, -force_dir2, 0, -force_dir1);
			dBodyAddTorque(feet_body, -torque_dir1, 0, torque_dir2);
		}
		moving = true;
	}
	if (keyboard::isDown('S')) {
		if (on_ground) {
			dBodyAddForce(head_body, force_dir2, 0, force_dir1);
			dBodyAddForce(feet_body, force_dir2, 0, force_dir1);
			dBodyAddTorque(feet_body, torque_dir1, 0, -torque_dir2);
		}
		moving = true;
	}
	if (mouse::isDown(mouse::Left)) {
		weapon->firePrimary();
	}
	else if (mouse::isDown(mouse::Right)) {
		weapon->fireSecondary();
	}
	if (!moving && false) {
		dBodySetAngularVel(head_body, 0, 0, 0);
	}
	if (on_ground) {
		const Vector3 vel = *(const Vector3 *)dBodyGetLinearVel(feet_body);
		dBodySetLinearVel(feet_body, 0.9f * vel[0], vel[1], 0.9f * vel[2]);
	}

	const Vector3 &angular_vel = *(const Vector3 *)dBodyGetAngularVel(feet_body);
	if (angular_vel * angular_vel > maximum_angular_vel * maximum_angular_vel) {
		Vector3 max;

		normalize(max, angular_vel);
		max * maximum_angular_vel;
		dBodySetAngularVel(feet_body, max[0], max[1], max[2]);  
	}

	const Vector3 &position = *(const Vector3 *)dBodyGetPosition(head_body);
	Vector3 direction = *(const Vector3 *)dBodyGetPosition(feet_body);

	direction -= position;
	direction[1] += head_position;

	direction += *(const Vector3 *)dBodyGetLinearVel(feet_body);
	dBodySetLinearVel(head_body, direction[0], direction[1], direction[2]);

	dGeomRaySet(line_of_sight, 
			position[0], position[1], position[2],
			look_direction[0], look_direction[1], look_direction[2]);

	if (jumping) {
		if (on_ground)
			jumping = keyboard::isDown(keyboard::Space);
	}
	else if (on_ground && keyboard::isDown(keyboard::Space)) {
		dBodyAddForce(head_body, 0, jump_force, 0);
		dBodyAddForce(feet_body, 0, jump_force, 0);
		jumping = true;
	}

	on_ground = false;

	target_body = 0;
	target_distance = dInfinity;

	weapon->update();
}

void FPSPlayer::update_look_direction(void)
{
	direction += mouse_turn_speed * mouse::getMovementX(); 
	pitch += mouse_turn_speed * mouse::getMovementY() * (mouse_invert ? 1 : -1); 
	if (pitch < -1.5f) {
		pitch = -1.5f;
	}
	else if (pitch > 1.5f) {
		pitch = 1.5f;
	}

	look_direction[0] = cosf(direction) * cosf(pitch);
	look_direction[1] = sinf(pitch);
	look_direction[2] = sinf(direction) * cosf(pitch);
}

const Vector3 &FPSPlayer::getEyePosition(void)
{
	return *(const Vector3 *)dBodyGetPosition(head_body);
}

void FPSPlayer::setOnGround(const Vector3 &normal)
{
	if (!on_ground) {
		ground_normal = normal;
		on_ground = true;
	}
	else if (ground_normal[1] < normal[1]) {
		ground_normal = normal;
	}

}

void FPSPlayer::setTarget(dBodyID body, const Vector3 &position, float distance)
{
	if (distance < target_distance) {
		target_body = body;
		target_position = position;
		target_distance = distance;
	}
}
