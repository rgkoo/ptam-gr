
#ifndef _include_FPSPlayer_hpp_
#define _include_FPSPlayer_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/math/Matrix.hpp"
#include "bleifrei/tex/TextureManager.hpp"

#include "Camera.hpp"
#include "ODEGeomData.hpp"
#include "Weapon.hpp"

class FPSPlayer {
public:
	FPSPlayer(const bleifrei::math::Vector3 &position, dWorldID world, dSpaceID space, bleifrei::tex::TextureManager &texture_manager);
	~FPSPlayer(void);

	void render(void);
	void update(float frame_step);
	void update_look_direction(void);

	void setOnGround(const bleifrei::math::Vector3 &normal);
	void setTarget(dBodyID body, const bleifrei::math::Vector3 &position, float distance);

	float getDirection(void) const											{ return direction;			}
	float getPitch(void) const												{ return pitch;				}

	const bleifrei::math::Vector3 &getEyePosition(void);
	const bleifrei::math::Vector3 &getLookDirection(void)					{ return look_direction;	}

	const bleifrei::math::Vector3 &getTargetPosition(void)					{ return target_position;	}
	float getTargetDistance(void)											{ return target_distance;	}
	dBodyID getTargetBody(void)												{ return target_body;		}

protected:
	ODEGeomData head_data, feet_data, line_of_sight_data;

	dGeomID head_geometry, feet_geometry, line_of_sight;
	dBodyID head_body, feet_body;

	bleifrei::math::Vector3 look_direction;

	bleifrei::math::Vector3 ground_normal;
	bool on_ground, jumping;

	float direction, pitch;

	dBodyID target_body;
	bleifrei::math::Vector3 target_position;
	float target_distance;

	Weapon *weapon;
};


#endif //_include_FPSPlayer_hpp_
