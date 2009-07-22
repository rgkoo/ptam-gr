
#ifndef _include_ODELight_hpp_
#define _include_ODELight_hpp_ 

// includes
#include <ode/ode.h>

#include "bleifrei/mesh/Model.hpp"

#include "Light.hpp"

class ODESphericalLight : public SphericalLight {
public:
	ODESphericalLight(dBodyID body, dGeomID geometry, bleifrei::mesh::Model model, const bleifrei::math::Vector3 &position, float radius = 0.25f)
		: SphericalLight(position, radius), body(body), geometry(geometry), model(model) {}

	void render(void);
	void update(void);
protected:
	dBodyID body;
	dGeomID geometry;

	bleifrei::mesh::Model model;
};

class ODERectangularLight : public RectangularLight {
public:
	ODERectangularLight(dBodyID body, dGeomID geometry, bleifrei::mesh::Model model, const bleifrei::math::Vector3 &position, float width = 0.25f, float length = 0.25f, float height = 0.25f)
		: RectangularLight(position, width, length), body(body), geometry(geometry), model(model), height(height) {}

	void render(void);
	void update(void);
protected:
	dBodyID body;
	dGeomID geometry;

	bleifrei::mesh::Model model;

	float height;
};

class ODETexturedLight : public TexturedLight {
public:
	ODETexturedLight(dBodyID body, dGeomID geometry, bleifrei::mesh::Model model, bleifrei::tex::Texture2 texture, bleifrei::tex::Texture2 coverage,
		const bleifrei::math::Vector3 &position, float width = 0.25f, float length = 0.25f, float height = 0.25f)
		: TexturedLight(texture, coverage, position, width, length), body(body), geometry(geometry), model(model), height(height) {}

	void render(void);
	void update(void);
protected:
	dBodyID body;
	dGeomID geometry;

	bleifrei::mesh::Model model;

	float height;
};

#endif // _include_ODELight_hpp_