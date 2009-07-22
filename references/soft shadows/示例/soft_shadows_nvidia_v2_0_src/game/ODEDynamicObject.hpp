
#ifndef _include_ODEDynamicObject_hpp_
#define _include_ODEDynamicObject_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/mesh/Model.hpp"

#include "Object.hpp"

class ODEDynamicObject : public Object {
public:
	ODEDynamicObject(dGeomID geometry, dBodyID body, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model)
		: geometry(geometry), body(body), Object(mesh, model), disable_steps(0) {}
	~ODEDynamicObject(void)	{}

	void update(void);
protected:
	dGeomID geometry;
	dBodyID body;

	int disable_steps;
};



#endif //_include_ODEDynamicObject_hpp_
