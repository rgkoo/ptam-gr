
// includes
#include <vector>

#include "bleifrei/gfx/opengl.hpp"
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/math/misc.hpp"

#include "FPSPlayer.hpp"
#include "TractorBeam.hpp"

using namespace bleifrei::math;
using namespace bleifrei::tex;
using namespace std;

const float beam_half_width = 0.5f;

TractorBeam::TractorBeam(FPSPlayer *player, Texture2 texture)
	: Weapon(player), texture(texture), primary_target(0), secondary_target(0), stop_primary(false), stop_secondary(false), no_throw(false)
{
}

TractorBeam::~TractorBeam(void)
{
}

void TractorBeam::update(void)
{
	if (secondary_target) {
		if (stop_secondary) {
			if (!no_throw) {
				dMass mass;
				dBodyGetMass(secondary_target, &mass);
				Vector3 force = player->getLookDirection() * 3500.0f * mass.mass;

				dBodyAddForce(secondary_target, force[0], force[1], force[2]);
			}
			dBodyEnable(secondary_target);
			secondary_target = 0;
		}
	}
	else if (primary_target) {
		if (stop_primary) {
			dBodyEnable(primary_target);
			primary_target = 0;
		}
	}
	stop_primary = true;
	stop_secondary = true;
}

void TractorBeam::firePrimary(void)
{
	if (primary_target) {
		dBodyGetRelPointPos(primary_target, target_position[0], target_position[1], target_position[2], &position[0]);

		Vector3 force = player->getEyePosition() + player->getLookDirection() * distance - position;

		dMass mass;
		dBodyGetMass(primary_target, &mass);
		float abs_force = length(force);
		if (abs_force > 5) {
			return;
		}
	
		force *= abs_force * mass.mass * 10;
		dBodyAddForceAtRelPos(primary_target, force[0],  force[1],  force[2], target_position[0], target_position[1], target_position[2]);
		dBodyEnable(primary_target);

		const Vector3 &linear_velocity = *(const Vector3 *)dBodyGetLinearVel(primary_target);
		dBodySetLinearVel(primary_target, 0.9f * linear_velocity[0], 0.9f * linear_velocity[1], 0.9f * linear_velocity[2]);

		const Vector3 &angular_velocity = *(const Vector3 *)dBodyGetAngularVel(primary_target);
		dBodySetAngularVel(primary_target, 0.9f * angular_velocity[0], 0.9f * angular_velocity[1], 0.9f * angular_velocity[2]);
	}
	else if (secondary_target == 0 && (primary_target = player->getTargetBody()) != 0) {
		distance = player->getTargetDistance();

		position = player->getTargetPosition();
		dBodyGetPosRelPoint(primary_target, position[0],  position[1],  position[2], &target_position[0]);
		dBodyEnable(primary_target);
	}
	stop_primary = false;
}

void TractorBeam::fireSecondary(void)
{
	if (secondary_target) {
		if (distance > 7.5f) distance -= 0.25f;
		if (distance < 7.2f) distance += 0.25f;

		dBodyGetRelPointPos(secondary_target, target_position[0], target_position[1], target_position[2], &position[0]);

		Vector3 force = player->getEyePosition() + player->getLookDirection() * distance - position;

		dMass mass;
		dBodyGetMass(secondary_target, &mass);
		float abs_force = length(force);
		if (abs_force > 5) {
			no_throw = true;
			return;
		}

		force *= abs_force * mass.mass * 10;
		dBodyAddForce(secondary_target, force[0],  force[1],  force[2]);
		dBodyEnable(secondary_target);

		const Vector3 &linear_velocity = *(const Vector3 *)dBodyGetLinearVel(secondary_target);
		dBodySetLinearVel(secondary_target, 0.9f * linear_velocity[0], 0.9f * linear_velocity[1], 0.9f * linear_velocity[2]);

		const Vector3 &angular_velocity = *(const Vector3 *)dBodyGetAngularVel(secondary_target);
		dBodySetAngularVel(secondary_target, 0.9f * angular_velocity[0], 0.9f * angular_velocity[1], 0.9f * angular_velocity[2]);
	}
	else if (primary_target == 0 && (secondary_target = player->getTargetBody()) != 0) {
		no_throw = false;
		distance = player->getTargetDistance();

		position = player->getTargetPosition();
		dBodyGetPosRelPoint(secondary_target, position[0],  position[1],  position[2], &target_position[0]);
		dBodyEnable(secondary_target);
	}
	stop_secondary = false;
}
