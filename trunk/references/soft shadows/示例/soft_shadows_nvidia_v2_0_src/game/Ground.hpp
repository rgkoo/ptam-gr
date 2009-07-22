
#ifndef _include_Ground_hpp_
#define _include_Ground_hpp_

// includes
#include <ode/ode.h>

#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/mesh/Model.hpp"

#include "ODEStaticObject.hpp"

class Ground : public ODEStaticObject {
public:
	Ground(dSpaceID space, bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model);
	~Ground(void);
protected:
	dTriMeshDataID tmData;
};

#endif //_include_Ground_hpp_
