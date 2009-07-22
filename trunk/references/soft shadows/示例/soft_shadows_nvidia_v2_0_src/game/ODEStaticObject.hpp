
#ifndef _include_ODEStaticObject_hpp_
#define _include_ODEStaticObject_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/mesh/Model.hpp"

#include "Object.hpp"
#include "ODEGeomData.hpp"

class ODEStaticObject : public Object {
public:
	ODEStaticObject(bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model);
	ODEStaticObject(dGeomID geometry, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model);
	~ODEStaticObject(void)	{}

	void setType(ODEGeomData::Type type)		{ geometry_data.setType(type);		}
protected:
	dGeomID geometry;
	ODEGeomData geometry_data;
};


#endif //_include_ODEStaticObject_hpp_
