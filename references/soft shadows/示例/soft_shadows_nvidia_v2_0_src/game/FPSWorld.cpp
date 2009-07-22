
// includes
#include <fstream>

#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/platform/platform.hpp"

#include "bleifrei/lang/Parser.hpp"
#include "bleifrei/io/console.hpp"

#include "Ground.hpp"
#include "ODEDynamicObject.hpp"
#include "ODEStaticObject.hpp"
#include "FPSWorld.hpp"
#include "PlayerCamera.hpp"
#include "PlayerObject.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::io;
using namespace bleifrei::lang;
using namespace bleifrei::math;
using namespace bleifrei::mesh;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

const float maximum_frame_step = 1 / 60.0f;
const int iterations = 10;

#define PHYSICS_SOLUTION_1

FPSWorld::FPSWorld(ConfigFile &config_file)
	: pause(false), accum_step(0)
{
	string map_mesh;
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contact_group = dJointGroupCreate(0);
	dWorldSetCFM(world, 1e-5f);
	dWorldSetERP(world, 0.2f);

	dWorldSetAutoEnableDepthSF1(world, 1);

	Mesh *mesh;
	Model model;
	Object *object;
	Light *light;

	dGeomID geometry;
	dBodyID body;
	dMass mass;

	Vector3 position;
	Vector3 gravity;
	Vector3 dimensions;
	int count;

	config_file.selectSection("Map");

	config_file.get("meshfile", map_mesh);
	config_file.get("gravity", gravity);
	setGravity(gravity);

	setMinFilter(Trilinear);
	setMagFilter(Bilinear);
	setAnisotropicFiltering(1000);

	mesh = mesh_manager.load(map_mesh, false);
	model = model_manager.load(mesh, false);
	object = new Ground(space, mesh, model);
	object->setShadowMode(Object::NoShadow);
	addObject(object);

	count = 0;
	config_file.selectSection("DynamicObjects");
	config_file.get("count", count);
	
	for (int i = 0; i < count; ++i) {
		std::string meshfile, type;
		double object_mass;
		bool disable = false;

		config_file.selectSection("DynamicObject", i);
		config_file.get("meshfile", meshfile);
		config_file.get("position", position);
		config_file.get("type", type);
		config_file.get("disable", disable);
		config_file.get("mass", object_mass);

		mesh = mesh_manager.load(meshfile, true);
		model = model_manager.load(mesh, false);
		mesh->cap();

		body = dBodyCreate(world);
		dBodySetPosition(body, position[0], position[1], position[2]);

		if (type == "box") {
			config_file.get("dimensions", dimensions);

			dMassSetBoxTotal(&mass, (float)object_mass, dimensions[0], dimensions[1], dimensions[2]);
			dBodySetMass(body, &mass);
			geometry = dCreateBox(space, dimensions[0], dimensions[1], dimensions[2]);
		}
		else if (type == "sphere") {
			double radius;
			config_file.get("radius", radius);

			dMassSetSphereTotal(&mass, (float)object_mass, 1);
			dBodySetMass(body, &mass);
			geometry = dCreateSphere(space, 1);
		}
		else {
			console::printf(1, 0, 0, "Error(ConfigFile): Unknown object type (%s)", type.c_str());
			return;
		}

		dGeomSetBody(geometry, body);
		if (disable)
			dBodyDisable(body);
		else
			dBodyEnable(body);

		addObject(new ODEDynamicObject(geometry, body, mesh, model));
	}

	count = 0;
	config_file.selectSection("StaticObjects");
	config_file.get("count", count);
	
	for (int i = 0; i < count; ++i) {
		std::string meshfile, type;

		config_file.selectSection("StaticObject", i);
		config_file.get("meshfile", meshfile);
		config_file.get("position", position);
		config_file.get("type", type);

		mesh = mesh_manager.load(meshfile);
		model = model_manager.load(mesh, false);

		if (type == "box") {
			config_file.get("dimensions", dimensions);

			geometry = dCreateBox(space, dimensions[0], dimensions[1], dimensions[2]);
		}
		else if (type == "sphere") {
			double radius;
			config_file.get("radius", radius);

			geometry = dCreateSphere(space, 1);
		}
		else {
			console::printf(1, 0, 0, "Error(ConfigFile): Unknown object type (%s)", type.c_str());
			return;
		}

		dGeomSetPosition(geometry, position[0], position[1], position[2]);

		ODEStaticObject *static_object = new ODEStaticObject(geometry, mesh, model);
		addObject(static_object);
	}

	config_file.selectSection("Player");
	config_file.get("position", position);
	player = new FPSPlayer(position, world, space, texture_manager);
	camera = new PlayerCamera(*player);

/*
	mesh = mesh_manager.load("ware.msh", false);
	model = model_manager.load(mesh, false);
	object = new Ground(space, mesh, model);
	object->setShadowMode(Object::NoShadow);
	addObject(object);
*/
	count = 0;
	config_file.selectSection("Lights");
	config_file.get("count", count);
	for (int i = 0; i < count; ++i) {
		string type;
		Vector4 diffuse, ambient;

		config_file.selectSection("Light", i);
		config_file.get("type", type);
		config_file.get("position", position);
		config_file.get("ambient", ambient);
		config_file.get("diffuse", diffuse);
		if (type == "spherical") {
			double radius = 0.25;

			config_file.get("radius", radius);

			light = new SphericalLight(position, (float)radius);
		}
		else {
			double width = 0.25;
			double length = 0.25;

			config_file.get("width", width);
			config_file.get("length", length);

			if (type == "rectangular") {
				light = new RectangularLight(position, (float)width, (float)length);
			}
			else if (type == "textured") {
				string texture, coverage;
				config_file.get("texture", texture);
				config_file.get("coverage", coverage);

				setMinFilter(Bilinear);
				setMagFilter(Bilinear);
				setAnisotropicFiltering(1);
				light = new TexturedLight(texture_manager.load(texture), texture_manager.load(coverage), position, (float)width, (float)length);
			}
			else {
				console::printf(1, 0, 0, "Error(ConfigFile): Unknown light type (%s)", type.c_str());
				return;
			}
		}
		light->setAmbientColor(ambient);
		light->setDiffuseColor(diffuse);
		addLight(light);
	}
	
	count = 0;
	config_file.selectSection("ODELights");
	config_file.get("count", count);
	for (int i = 0; i < count; ++i) {
		string meshfile, type;
		Vector4 diffuse, ambient;
		bool fixed = false;
		float light_mass;

		config_file.selectSection("ODELight", i);
		config_file.get("meshfile", meshfile);
		config_file.get("type", type);
		config_file.get("mass", light_mass);
		config_file.get("position", position);
		config_file.get("ambient", ambient);
		config_file.get("diffuse", diffuse);
		config_file.get("fixed", fixed);

		mesh = mesh_manager.load(meshfile);
		model = model_manager.load(mesh, false);

		if (type == "spherical") {
			float radius = 0.25;

			config_file.get("radius", radius);

			body = dBodyCreate(world);
			dMassSetSphereTotal(&mass, light_mass, radius);
			dBodySetMass(body, &mass);
			geometry = dCreateSphere(space, radius);
			dGeomSetBody(geometry, body);

			light = new ODESphericalLight(body, geometry, model, position, radius);
		}
		else {
			float width = 0.25;
			float length = 0.25;
			float height = 0.25;

			config_file.get("width", width);
			config_file.get("length", length);
			config_file.get("height", height);

			body = dBodyCreate(world);
			dMassSetBoxTotal(&mass, light_mass, width, height, length);
			dBodySetMass(body, &mass);
			geometry = dCreateBox(space, width, height, length);
			dGeomSetBody(geometry, body);

			if (type == "rectangular") {
				light = new ODERectangularLight(body, geometry, model, position, width, length, height);
			}
			else if (type == "textured") {
				string texture, coverage;
				config_file.get("texture", texture);
				config_file.get("coverage", coverage);

				setMinFilter(Bilinear);
				setMagFilter(Bilinear);
				setAnisotropicFiltering(1);
				light = new ODETexturedLight(body, geometry, model, texture_manager.load(texture), texture_manager.load(coverage), position, width, length, height);
			}
			else {
				console::printf(1, 0, 0, "Error(ConfigFile): Unknown light type (%s)", type.c_str());
				return;
			}
		}
		light->setAmbientColor(ambient);
		light->setDiffuseColor(diffuse);
		addLight(light);

		dBodySetPosition(body, position[0], position[1], position[2]);

		if (fixed) {
			Vector3 anchor;

			config_file.get("anchor", anchor);
			
			dJointID joint = dJointCreateBall(world, 0);
			dJointAttach(joint, 0, body);
			dJointSetBallAnchor(joint, anchor[0], anchor[1], anchor[2]);
		}
	}
}

FPSWorld::~FPSWorld(void)
{
	delete camera;
	deleteObjects();
	deleteLights();

	dJointGroupDestroy(contact_group);
	dSpaceDestroy(space);
	dWorldDestroy(world);
}

unsigned int FPSWorld::getLoadSteps(void)
{
	return World::getLoadSteps() + 8;
}

Vector3 sight_color(1,1,1);

void FPSWorld::render(void)
{
	RenderTarget::setActive(position_buffer);
	glLoadMatrixf(&camera->getMatrix()[0]);

	RenderTarget::setActive(luminance_buffer);
	glLoadMatrixf(&camera->getMatrix()[0]);

	RenderTarget::setActive(screen_buffer);
	glLoadMatrixf(&camera->getMatrix()[0]);

	RenderTarget::setActive(0);
	glLoadMatrixf(&camera->getMatrix()[0]);

	World::render();

	player->render();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderTarget::enableOrtho(platform::getScreenWidth(), platform::getScreenHeight());

	Texture2::disable();
	glPointSize(2);
	glColor4f(sight_color[0],sight_color[1],sight_color[2],0.8f);
	glBegin(GL_POINTS);
		glVertex2i(platform::getScreenWidth() / 2, platform::getScreenHeight() / 2);
	glEnd();
	Texture2::enable();

	RenderTarget::disableOrtho();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void FPSWorld::update(void)
{
	float frame_step = platform::getFrameStep();

	sight_color = Vector3(1,1,1);

	static bool toggleP = false;
	if (GetAsyncKeyState('P') & 0x8000) {
		if (!toggleP) {
			pause = !pause;
			toggleP = true;
		}
	}
	else toggleP = false;

	if (!pause) {

	accum_step += frame_step;
	
	player->update_look_direction();

#ifdef PHYSICS_SOLUTION_1
	for (; accum_step > maximum_frame_step; accum_step -= maximum_frame_step) {
		dSpaceCollide(space, this, collide_callback);
		dWorldStepFast1(world, maximum_frame_step, iterations);
		dJointGroupEmpty(contact_group);
		World::update();
		player->update(maximum_frame_step);
	}
#else
	if (accum_step >= maximum_frame_step) {
		dSpaceCollide(space, this, collide_callback);
		dWorldStepFast1(world, maximum_frame_step, iterations);
		dJointGroupEmpty(contact_group);
		World::update();
		player->update(maximum_frame_step);
		accum_step -= maximum_frame_step;
	}
#endif
	}

	camera->update();
}

void FPSWorld::setGravity(const Vector3 &gravity)
{
	this->gravity = gravity;
	dWorldSetGravity(world, gravity[0], gravity[1], gravity[2]);
	up_direction = -gravity;
	normalize(up_direction);
}

void FPSWorld::collide_callback(void *world, dGeomID object1, dGeomID object2)
{
	((FPSWorld *)world)->collide_callback(object1, object2);
}

inline void FPSWorld::collide_callback(dGeomID object1, dGeomID object2)
{
	const int max_contacts = 15;
	dContact contacts[max_contacts];

	ODEGeomData *object_data1 = (ODEGeomData *)dGeomGetData(object1);
	ODEGeomData *object_data2 = (ODEGeomData *)dGeomGetData(object2);

	dBodyID body1 = dGeomGetBody(object1);
	dBodyID body2 = dGeomGetBody(object2);
	
	int type1 = object_data1 ? object_data1->getType() : ODEGeomData::Unknown;
	int type2 = object_data2 ? object_data2->getType() : ODEGeomData::Unknown;

	if (type1 == ODEGeomData::LineOfSight) {
		if (body2 && type2 != ODEGeomData::PlayerHead && type2 != ODEGeomData::PlayerFeet && dCollide(object1, object2, 1, &contacts[0].geom, sizeof(dContact))) {
			sight_color = Vector3(0,1,0);

			((FPSPlayer *)object_data1->getOwner())->setTarget(body2, *(const Vector3 *)contacts[0].geom.pos, contacts[0].geom.depth);
		}
	}
	else if (type2 == ODEGeomData::LineOfSight) {
		if (body1 && type1 != ODEGeomData::PlayerHead && type1 != ODEGeomData::PlayerFeet && dCollide(object1, object2, 1, &contacts[0].geom, sizeof(dContact))) {
			sight_color = Vector3(0,1,0);

			((FPSPlayer *)object_data2->getOwner())->setTarget(body1, *(const Vector3 *)contacts[0].geom.pos, contacts[0].geom.depth);
		}
	}
	else if (body1 || body2) {
		int num_contacts = dCollide(object1, object2, max_contacts, &contacts[0].geom, sizeof(dContact));

		for (int contact = 0; contact < num_contacts; ++contact) {
			contacts[contact].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contacts[contact].surface.mu = (type1 == ODEGeomData::PlayerFeet || type2 == ODEGeomData::PlayerFeet) ? 1.5f : 0.5f;
			contacts[contact].surface.slip1 = 0.00f;
			contacts[contact].surface.slip2 = 0.00f;
			contacts[contact].surface.soft_erp = 0.2f;
			contacts[contact].surface.soft_cfm = 0.01f;

			dJointID contact_joint = dJointCreateContact(world, contact_group, contacts + contact);
			dJointAttach(contact_joint, body1, body2);
		}
		if (num_contacts) {
			if (type1 == ODEGeomData::PlayerFeet) {
				if (type2 == ODEGeomData::Finish) pause = true;
				((FPSPlayer *)object_data1->getOwner())->setOnGround(contacts[0].geom.normal);
			}
			else if (type2 == ODEGeomData::PlayerFeet) {
				if (type1 == ODEGeomData::Finish) pause = true;
				((FPSPlayer *)object_data2->getOwner())->setOnGround(-Vector3(contacts[0].geom.normal));
			}
		}
	}
}
