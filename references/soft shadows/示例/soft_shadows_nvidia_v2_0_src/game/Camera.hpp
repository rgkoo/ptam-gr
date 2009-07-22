
#ifndef _include_Camera_hpp_
#define _include_Camera_hpp_ 


// includes
#include "bleifrei/math/Matrix.hpp"

class Camera {
public:
	Camera(void)
	{
		matrix.identity();
	}
	virtual ~Camera(void);

	virtual void update(void);

	const bleifrei::math::Matrix4x4 &getMatrix(void)		{ return matrix;		}
protected:
	bleifrei::math::Matrix4x4 matrix;
};


#endif //_include_Camera_hpp_
