
#ifndef _include_Light_hpp_
#define _include_Light_hpp_ 


// includes
#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/tex/Texture.hpp"

class Light {
public:
	enum Type {
		Unknown,
		Spherical,
		Rectangular,
		Textured
	};

	Light(void)	: diffuse(0.5f, 0.5f, 0.5f, 0.5f) {}
	virtual ~Light(void) {}

	virtual void render(void);
	virtual void update(void);

	Type getType(void)											{ return type;							}

	void setDiffuseColor(bleifrei::math::Vector4 &diffuse)		{ this->diffuse = diffuse;				}
	void setAmbientColor(bleifrei::math::Vector4 &ambient)		{ this->ambient = ambient;				}
	void setPosition(bleifrei::math::Vector3 &position)
	{
		bleifrei::math::assign(this->position, position);
	}

	const bleifrei::math::Vector4 &getDiffuseColor(void)		{ return diffuse;						}
	const bleifrei::math::Vector4 &getAmbientColor(void)		{ return ambient;						}
	const bleifrei::math::Vector4 &getPosition(void)			{ return position;						}
	const bleifrei::math::Matrix4x4 &getMatrix(void)			{ return matrix;						}
protected:
	Light(const bleifrei::math::Vector3 &position, Type type = Unknown);

	bleifrei::math::Vector4 diffuse;
	bleifrei::math::Vector4 ambient;
	bleifrei::math::Vector4 position;
	bleifrei::math::Matrix4x4 matrix;
	Type type;
};

class SphericalLight : public Light {
public:
	SphericalLight(const bleifrei::math::Vector3 &position, float radius = 0.25f)
		: Light(position, Spherical), radius(radius) {}

	float getRadius(void)										{ return radius;				}
protected:
	float radius;
};

class RectangularLight : public Light {
public:
	RectangularLight(const bleifrei::math::Vector3 &position, float width = 0.25f, float length = 0.25f);

	void update(void);

	float getWidth(void)										{ return width;					}
	float getLength(void)										{ return length;				}
	const bleifrei::math::Matrix4x4 &getMatrixRotInverse(void)	{ return matrix_rot_inverse;	}
protected:
	bleifrei::math::Matrix4x4 matrix_rot_inverse;
	float width, length;
};

class TexturedLight : public RectangularLight {
public:
	TexturedLight(bleifrei::tex::Texture2 texture, bleifrei::tex::Texture2 coverage, const bleifrei::math::Vector3 &position, float width = 0.25f, float length = 0.25f);

	bleifrei::tex::Texture2 getTexture(void)					{ return texture;				}
	bleifrei::tex::Texture2 getCoverageTexture(void)			{ return coverage;				}
protected:
	bleifrei::tex::Texture2 texture;
	bleifrei::tex::Texture2 coverage;
};

#endif //_include_Light_hpp_
