
#ifndef _include_PlayerObject_hpp_
#define _include_PlayerObject_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/math/Matrix.hpp"

#include "Object.hpp"
#include "ODEGeomData.hpp"

class PlayerObject : public Object {
public:
	PlayerObject(dGeomID geometry, dBodyID body, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model);
	~PlayerObject(void) {}

	void update(void);

	void setOnGround(const bleifrei::math::Vector3 &normal);

	float getDirection(void)							{ return direction;												}
	const bleifrei::math::Vector3 &getPosition(void)	{ return *(bleifrei::math::Vector3 *)dBodyGetPosition(body);	}
protected:
	dGeomID geometry;
	dBodyID body;

	ODEGeomData data;

	bleifrei::math::Vector3 surface_normal;

	float direction;
	bool on_ground;
	bool jumping;
};


#endif //_include_PlayerObject_hpp_