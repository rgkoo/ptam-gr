
#ifndef _include_ODEWorld_hpp_
#define _include_ODEWorld_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/math/Matrix.hpp"
#include "bleifrei/io/ConfigFile.hpp"

#include "PlayerCamera.hpp"
#include "World.hpp"

class ODEWorld : public World {
public:
	ODEWorld(bleifrei::io::ConfigFile &config_file);
	~ODEWorld(void);

	unsigned int getLoadSteps(void);

	void render(void);
	void update(void);

	void setGravity(const bleifrei::math::Vector3 &gravity);
protected:
	static void collide_callback(void *data, dGeomID object1, dGeomID object2);
	void collide_callback(dGeomID object1, dGeomID object2);

	bool pause;

	float accum_step;

	dWorldID world;
	dSpaceID space;
	dJointGroupID contact_group;

	bleifrei::math::Vector3 gravity;
	bleifrei::math::Vector3 up_direction;

	PlayerCamera *camera;
};


#endif //_include_ODEWorld_hpp_
