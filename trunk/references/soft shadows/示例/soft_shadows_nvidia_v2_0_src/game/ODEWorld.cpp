
// includes
#include <fstream>

#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/platform/platform.hpp"

#include "bleifrei/lang/Parser.hpp"
#include "bleifrei/io/console.hpp"

#include "Ground.hpp"
#include "ODEDynamicObject.hpp"
#include "ODEStaticObject.hpp"
#include "ODEWorld.hpp"
#include "PlayerObject.hpp"

using namespace bleifrei::io;
using namespace bleifrei::lang;
using namespace bleifrei::math;
using namespace bleifrei::mesh;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

const float maximum_frame_step = 1 / 60.0f;
const int iterations = 10;

//#define PHYSICS_SOLUTION_1

ODEWorld::ODEWorld(ConfigFile &config_file)
	: pause(false), accum_step(0)
{
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contact_group = dJointGroupCreate(0);
	dWorldSetCFM(world, 1e-5f);
	dWorldSetERP(world, 0.2f);

	dWorldSetAutoEnableDepthSF1(world, 1);

	Mesh *mesh;
	Model model;
	Object *object;
	PlayerObject *player;
	Light *light;

	dGeomID geometry;
	dBodyID body;
	dMass mass;

	Vector3 position;
	Vector3 gravity;
	Vector3 dimensions;
	int count = 0;

	config_file.selectSection("Map");

	config_file.get("gravity", gravity);
	setGravity(gravity);

	setMinFilter(Trilinear);
	setMagFilter(Bilinear);
	setAnisotropicFiltering(1000);

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

		mesh = mesh_manager.load(meshfile);
		model = model_manager.load(mesh, false);

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

	config_file.selectSection("StaticObjects");
	config_file.get("count", count);
	
	for (int i = 0; i < count; ++i) {
		std::string meshfile, type;
		bool finish = false;

		config_file.selectSection("StaticObject", i);
		config_file.get("meshfile", meshfile);
		config_file.get("position", position);
		config_file.get("type", type);
		config_file.get("finish", finish);

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
		if (finish) {
			static_object->setType(ODEGeomData::Finish);
		}
		addObject(static_object);
	}

	config_file.selectSection("Player");
	config_file.get("position", position);

	mesh = mesh_manager.load("sphere-2.msh");
	model = model_manager.load(mesh, false);

	body = dBodyCreate(world);
	dBodySetPosition(body, position[0], position[1], position[2]);
	dMassSetSphereTotal(&mass, 0.75f, 1);
	dBodySetMass(body, &mass);
	geometry = dCreateSphere(space, 1);
	dGeomSetBody(geometry, body);

	player = new FPSPlayer(geometry, body, mesh, model);
	addObject(player);
	camera = new PlayerCamera(*player);

	mesh = mesh_manager.load("ware.msh", false);
	model = model_manager.load(mesh, false);
	object = new Ground(space, mesh, model);
	object->setShadowMode(Object::NoShadow);
	addObject(object);

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
}

ODEWorld::~ODEWorld(void)
{
	delete camera;
	deleteObjects();
	deleteLights();

	dJointGroupDestroy(contact_group);
	dSpaceDestroy(space);
	dWorldDestroy(world);
}

unsigned int ODEWorld::getLoadSteps(void)
{
	return World::getLoadSteps() + 8;
}

void ODEWorld::render(void)
{
	glLoadMatrixf(&camera->getMatrix()[0]);
	World::render();
}

void ODEWorld::update(void)
{
	float frame_step = platform::getFrameStep();

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
#ifdef PHYSICS_SOLUTION_1
	for (; accum_step > maximum_frame_step; accum_step -= maximum_frame_step) {
		dSpaceCollide(space, this, collide_callback);
		//dWorldStepFast1(world, maximum_frame_step, iterations);
		dWorldStep(world, maximum_frame_step);
		dJointGroupEmpty(contact_group);
	}
//	dSpaceCollide(space, this, collide_callback);
	//dWorldStepFast1(world, frame_step, iterations);
//	dWorldStep(world, maximum_frame_step);
//	dJointGroupEmpty(contact_group);
#else
	if (accum_step >= maximum_frame_step) {
		dSpaceCollide(space, this, collide_callback);
		dWorldStepFast1(world, maximum_frame_step, iterations);
		//dWorldStep(world, maximum_frame_step);
		dJointGroupEmpty(contact_group);
		accum_step -= maximum_frame_step;
	}
#endif
	}

	camera->update();
	World::update();
}

void ODEWorld::setGravity(const Vector3 &gravity)
{
	this->gravity = gravity;
	dWorldSetGravity(world, gravity[0], gravity[1], gravity[2]);
	up_direction = -gravity;
	normalize(up_direction);
}

void ODEWorld::collide_callback(void *world, dGeomID object1, dGeomID object2)
{
	((ODEWorld *)world)->collide_callback(object1, object2);
}

inline void ODEWorld::collide_callback(dGeomID object1, dGeomID object2)
{
	const int max_contacts = 15;
	dContact contacts[max_contacts];

	ODEGeomData *object_data1 = (ODEGeomData *)dGeomGetData(object1);
	ODEGeomData *object_data2 = (ODEGeomData *)dGeomGetData(object2);

	dBodyID body1 = dGeomGetBody(object1);
	dBodyID body2 = dGeomGetBody(object2);
	
	int type1 = object_data1 ? object_data1->getType() : ODEGeomData::Unknown;
	int type2 = object_data2 ? object_data2->getType() : ODEGeomData::Unknown;

	if (body1 || body2) {
		int num_contacts = dCollide(object1, object2, max_contacts, &contacts[0].geom, sizeof(dContact));

		for (int contact = 0; contact < num_contacts; ++contact) {
			contacts[contact].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
			contacts[contact].surface.mu = (type1 == ODEGeomData::Player || type2 == ODEGeomData::Player) ? 1.5f : 0.5f;
			contacts[contact].surface.slip1 = 0.00f;
			contacts[contact].surface.slip2 = 0.00f;
			contacts[contact].surface.soft_erp = 0.2f;
			contacts[contact].surface.soft_cfm = 0.01f;

			dJointID contact_joint = dJointCreateContact(world, contact_group, contacts + contact);
			dJointAttach(contact_joint, body1, body2);
		}
		if (num_contacts) {
			if (type1 == ODEGeomData::Player) {
				if (type2 == ODEGeomData::Finish) pause = true;
				((PlayerObject *)object_data1->getOwner())->setOnGround(contacts[0].geom.normal);
			}
			else if (type2 == ODEGeomData::Player) {
				if (type1 == ODEGeomData::Finish) pause = true;
				((PlayerObject *)object_data2->getOwner())->setOnGround(-Vector3(contacts[0].geom.normal));
			}
		}
	}
}
