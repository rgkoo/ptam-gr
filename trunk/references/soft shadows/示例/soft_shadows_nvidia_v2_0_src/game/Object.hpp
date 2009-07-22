
#ifndef _include_Object_hpp_
#define _include_Object_hpp_ 


// includes
#include "bleifrei/math/Matrix.hpp"
#include "bleifrei/mesh/Mesh.hpp"
#include "bleifrei/mesh/Model.hpp"

class Object {
public:
	enum ShadowMode {
		SoftShadow,
		HardShadow,
		NoShadow
	};

	Object(bleifrei::mesh::Mesh *mesh, bleifrei::mesh::Model model);
	virtual ~Object(void);

	virtual void render(void);
	virtual void update(void);

	void setShadowMode(ShadowMode mode)							{ shadow_mode = mode;		}
	ShadowMode getShadowMode(void)								{ return shadow_mode;		}
	bleifrei::mesh::Mesh *getMesh(void)							{ return mesh;				}

	const bleifrei::math::Matrix4x4 &getMatrix(void)			{ return matrix;			}
	const bleifrei::math::Matrix4x4 &getMatrixInverse(void)		{ return matrix_inverse;	}
protected:
	ShadowMode shadow_mode;

	bleifrei::math::Matrix4x4 matrix;
	bleifrei::math::Matrix4x4 matrix_inverse;

	bleifrei::mesh::Mesh *mesh;
	bleifrei::mesh::Model model;
};

#endif //_include_Object_hpp_ 
