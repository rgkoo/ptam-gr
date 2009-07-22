
// includes
#include <fstream>

#include "bleifrei/math/Matrix_op.hpp"
#include "bleifrei/io/console.hpp"
#include "bleifrei/platform/platform.hpp"

#include "World.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::io;
using namespace bleifrei::math;
using namespace bleifrei::mesh;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

#define MULTIPLE_LIGHTS

World::World(void)
	: mesh_manager("../meshes/"), texture_manager("../textures/"), model_manager(texture_manager), zfail(true)
{
	if (GL_ARB_vertex_program_initialized) {
		vp_light = VertexProgram(ifstream("vp/light.vp"));
		vp_posworld = VertexProgram(ifstream("vp/posworld.vp"));
		vp_shadowvolume = VertexProgram(ifstream("vp/shadowvolume.vp"));
		vp_wedge = VertexProgram(ifstream("vp/wedge.vp"));
	}

	if (GL_ARB_fragment_program_initialized) {
		fp_color = FragmentProgram(ifstream("fp/color.fp"));
		fp_light = FragmentProgram(ifstream("fp/light.fp"));
		fp_light_rect = FragmentProgram(ifstream("fp/light_rect.fp"));
		fp_posworld = FragmentProgram(ifstream("fp/posworld.fp"));
		fp_rectwedge = FragmentProgram(ifstream("fp/rectwedge.fp"));
		fp_spherewedge = FragmentProgram(ifstream("fp/spherewedge.fp"));
		fp_texturedwedge = FragmentProgram(ifstream("fp/texturedwedge.fp"));
	}

	setMinFilter(Nearest);
	setMagFilter(Nearest);
	setAnisotropicFiltering(1);

	atan2_texture = createAtan2Cubemap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	color2xlookup_texture = create2xColorLookupTexture();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	setMinFilter(Nearest);
	setMagFilter(Nearest);
	setAnisotropicFiltering(1);

	colorlookup_texture = createColorLookupTexture();
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	setMinFilter(Bilinear);
	setMagFilter(Bilinear);
	sphericalarea_texture = createSphericalAreaTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	setMinFilter(Bilinear);
	setMagFilter(Bilinear);
	rectarea_texture = createRectAreaTexture();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	console::print(1.0f, 1.0f, 1.0f, "Creating position render target");
	position_texture.create();
	position_buffer = new RenderTarget(platform::getScreenWidth(), platform::getScreenHeight(), RenderTarget::rgb16frect);
	RenderTarget::setActive(position_buffer);
	FragmentProgram::enable();
	VertexProgram::enable();

	console::print(1.0f, 1.0f, 1.0f, "Creating v-buffer render target");
	additive_luminance_texture.create();
	subtractive_luminance_texture.create();
	luminance_buffer = new RenderTarget(platform::getScreenWidth(), platform::getScreenHeight(), RenderTarget::rgba8dbrect);
	RenderTarget::setActive(luminance_buffer);
	FragmentProgram::enable();
	VertexProgram::enable();

	console::print(1.0f, 1.0f, 1.0f, "Creating screen render target");
	screen_texture.create();
	screen_buffer = new RenderTarget(platform::getScreenWidth(), platform::getScreenHeight(), RenderTarget::rgba8rect);
	RenderTarget::setActive(screen_buffer);
	FragmentProgram::enable();
	VertexProgram::enable();

	RenderTarget::setActive(0);
}

unsigned int World::getLoadSteps(void)
{
	return 8;
}

World::~World(void)
{
	if (GL_ARB_vertex_program_initialized) {
		vp_light.destroy();
		vp_posworld.destroy();
		vp_shadowvolume.destroy();
		vp_wedge.destroy();
	}

	if (GL_ARB_fragment_program_initialized) {
		fp_color.destroy();
		fp_light.destroy();
		fp_light_rect.destroy();
		fp_posworld.destroy();
		fp_rectwedge.destroy();
		fp_spherewedge.destroy();
		fp_texturedwedge.destroy();
	}

	delete position_buffer;
	position_texture.destroy();

	delete luminance_buffer;
	additive_luminance_texture.destroy();
	subtractive_luminance_texture.destroy();

	delete screen_buffer;
	screen_texture.destroy();

	atan2_texture.destroy();
	sphericalarea_texture.destroy();
	rectarea_texture.destroy();
	color2xlookup_texture.destroy();
	colorlookup_texture.destroy();
}

void World::render(void)
{
#ifndef MULTIPLE_LIGHTS
	VertexProgram::enable();
	FragmentProgram::enable();
	render(0, lights[0]);
	FragmentProgram::disable();
	VertexProgram::disable();
#else
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Light *>::iterator it = lights.begin();
	VertexProgram::enable();
	FragmentProgram::enable();
	render(0, *it);
	FragmentProgram::disable();
	VertexProgram::disable();

	for (++it; it != lights.end(); ++it) {
		render(screen_buffer, *it);

		RenderTarget::setActive(0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glColor4f(1,1,1,1);

		TextureRect::enable();
		screen_texture.bind();
		screen_buffer->bind();
		RenderTarget::enableOrtho(platform::getScreenWidth(), platform::getScreenHeight());
		TextureRect::blit(0, 0, platform::getScreenWidth(), platform::getScreenHeight(), 0, 0, (float)platform::getScreenWidth(), (float)platform::getScreenHeight());
		RenderTarget::disableOrtho();
		screen_buffer->release();
		TextureRect::disable();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
#endif

	glColor3f(1,1,1);
	Texture2::enable();
	for (vector<Light *>::iterator it = lights.begin(); it != lights.end(); ++it) {
		(*it)->render();
	}
	Texture2::disable();
}

void World::render(RenderTarget *render_target, Light *light)
{
	RenderTarget::setActive(position_buffer);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vp_posworld.bind();
	fp_posworld.bind();

	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		VertexProgram::localParameter(0, (*it)->getMatrix());	
		(*it)->render();
	}

	RenderTarget::setActive(luminance_buffer);

	position_texture.bind();
	position_buffer->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	bool clear = true;
	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it, clear = false) {
		render_shadow(*it, light, clear);
	}

	RenderTarget::setActive(render_target);

	vp_light.bind();
	if (light->getType() == Light::Spherical) {
		fp_light.bind();
	}
	else {
		fp_light_rect.bind();

		FragmentProgram::localParameter(3, light->getMatrix().getColumn(1));
	}

	FragmentProgram::localParameter(0, light->getPosition());
	FragmentProgram::localParameter(1, light->getDiffuseColor());
	FragmentProgram::localParameter(2, light->getAmbientColor());

	setUnit(2);
	subtractive_luminance_texture.bind();
	luminance_buffer->bind(RenderTarget::Front);

	setUnit(1);
	additive_luminance_texture.bind();
	luminance_buffer->bind(RenderTarget::Back);

	setUnit(0);
	position_texture.bind();
	position_buffer->bind();

	setUnit(3);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		VertexProgram::localParameter(0, (*it)->getMatrix());
		(*it)->render();
	}

	setUnit(0);

	position_buffer->release();
	luminance_buffer->release(RenderTarget::Front);
	luminance_buffer->release(RenderTarget::Back);
}

void World::update(void)
{
	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->update();
	}
	for (vector<Light *>::iterator it = lights.begin(); it != lights.end(); ++it) {
		(*it)->update();
	}
}

void World::deleteObjects(void)
{
	for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
		delete *it;
	}
	objects.clear();
}

void World::deleteLights(void)
{
	for (vector<Light *>::iterator it = lights.begin(); it != lights.end(); ++it) {
		delete *it;
	}
	lights.clear();
}

void World::render_umbra_zpass(Object *object)
{
	glDepthMask(GL_FALSE);

	glDrawBuffer(GL_FRONT);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE);

	vp_shadowvolume.bind();
	fp_color.bind();
	FragmentProgram::localParameter(0, 8.0f / 255.0f);

	glFrontFace(GL_CCW);

	glPushMatrix();
		glMultMatrixf(&object->getMatrix()[0]);

		shadow_volume.renderUmbraZPass();
	glPopMatrix();

	glDrawBuffer(GL_BACK);

	glFrontFace(GL_CW);

	glPushMatrix();
		glMultMatrixf(&object->getMatrix()[0]);

		shadow_volume.renderUmbraZPass();
	glPopMatrix();

	glDisable(GL_BLEND);
	glFrontFace(GL_CCW);

	glDepthMask(GL_TRUE);
}

void World::render_umbra_zfail(Object *object)
{
	glDepthMask(GL_FALSE);
	glDrawBuffer(GL_FRONT);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_ONE, GL_ONE);

	vp_shadowvolume.bind();
	fp_color.bind();
	FragmentProgram::localParameter(0, 8.0f / 255.0f);

	glFrontFace(GL_CW);
	glDepthFunc(GL_GEQUAL);

	glPushMatrix();
		glMultMatrixf(&object->getMatrix()[0]);

		shadow_volume.renderUmbraZFail();

	glPopMatrix();

	glDrawBuffer(GL_BACK);

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GEQUAL);
	glFrontFace(GL_CCW);

	glPushMatrix();
		glMultMatrixf(&object->getMatrix()[0]);

		shadow_volume.renderUmbraZFail();
	glPopMatrix();

	// depth mask?
	glDisable(GL_BLEND);
	glFrontFace(GL_CCW);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

inline void World::render_sub_luminance_zpass(unsigned int numwedges)
{
	glDrawBuffer(GL_FRONT);
	for (unsigned int i = 0; i < numwedges; i++) {
		glDisable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

		glActiveStencilFaceEXT(GL_BACK);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);
		glActiveStencilFaceEXT(GL_FRONT);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		FragmentProgram::disable();

		shadow_volume.renderWedgeFrontCCW(i);

		FragmentProgram::enable();

		glEnable(GL_CULL_FACE);

		glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

		shadow_volume.sendEdgeToFragmentProgram(i);
		shadow_volume.renderWedgeFrontCCW(i);
	}
}

inline void World::render_sub_luminance_zfail(unsigned int numwedges)
{
	glDrawBuffer(GL_FRONT);
	for (unsigned int i = 0; i < numwedges; i++) {
		glDisable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

		glActiveStencilFaceEXT(GL_BACK);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);

		glActiveStencilFaceEXT(GL_FRONT);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		FragmentProgram::disable();

		shadow_volume.renderWedgeFrontCW(i);

		FragmentProgram::enable();

		glEnable(GL_CULL_FACE);

		glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

		shadow_volume.sendEdgeToFragmentProgram(i);
		shadow_volume.renderWedgeFrontCW(i);
	}
}

inline void World::render_add_luminance_zpass(unsigned int numwedges)
{
	glDrawBuffer(GL_BACK);
	for (unsigned int i = 0; i < numwedges; i++) {
		glDisable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

		glActiveStencilFaceEXT(GL_BACK);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);

		glActiveStencilFaceEXT(GL_FRONT);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		FragmentProgram::disable();
		shadow_volume.renderWedgeBack(i);
		FragmentProgram::enable();

		glEnable(GL_CULL_FACE);

		glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

		shadow_volume.sendEdgeToFragmentProgram(i);
		shadow_volume.renderWedgeBack(i);
	}
}

inline void World::render_add_luminance_zfail(unsigned int numwedges)
{
	glDrawBuffer(GL_BACK);
	for (unsigned int i = 0; i < numwedges; i++) {
		glDisable(GL_CULL_FACE);

		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);

		glActiveStencilFaceEXT(GL_BACK);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);

		glActiveStencilFaceEXT(GL_FRONT);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		FragmentProgram::disable();
		shadow_volume.renderWedgeBack(i);
		FragmentProgram::enable();

		glEnable(GL_CULL_FACE);

		glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

		shadow_volume.sendEdgeToFragmentProgram(i);
		shadow_volume.renderWedgeBack(i);
	}
}

void World::render_penumbra(Object *object, Light *light)
{
	vp_wedge.bind();
	setUnit(0);
	position_texture.bind();

	if (light->getType() == Light::Spherical) {
		setUnit(1);
		atan2_texture.bind();

		setUnit(2);
		sphericalarea_texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		setUnit(3);
		color2xlookup_texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		setUnit(0);

		fp_spherewedge.bind();

		Vector4 lightvector = light->getPosition();
		lightvector[3] = 1.0f / ((SphericalLight *)light)->getRadius();
		FragmentProgram::localParameter(0, lightvector);
		VertexProgram::localParameter(0, object->getMatrix());
	}
	else if (light->getType() == Light::Rectangular) {
		RectangularLight *rectangular_light = (RectangularLight *)light;

		setUnit(1);
		atan2_texture.bind();

		setUnit(2);
		rectarea_texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		setUnit(3);
		colorlookup_texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		setUnit(0);

		fp_rectwedge.bind();

		FragmentProgram::localParameter(0, rectangular_light->getMatrixRotInverse());
		FragmentProgram::localParameter(4, 1.0f / (float)rectangular_light->getWidth(), 0.0f, 1.0f / (float)rectangular_light->getLength(), 0.0f);
		VertexProgram::localParameter(0, rectangular_light->getMatrixRotInverse() * object->getMatrix());
	}
	else if (light->getType() == Light::Textured) {
		TexturedLight *textured_light = (TexturedLight *)light;

		setUnit(1);
		textured_light->getCoverageTexture().bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		setUnit(2);
		colorlookup_texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		setUnit(0);

		fp_texturedwedge.bind();

		FragmentProgram::localParameter(0, textured_light->getMatrixRotInverse());
		FragmentProgram::localParameter(4, 1.0f / (float)textured_light->getWidth(), 0.0f, 1.0f / (float)textured_light->getLength(), 0.0f);
		VertexProgram::localParameter(0, textured_light->getMatrixRotInverse() * object->getMatrix());
	}
	else return;

	glPushMatrix();
		glMultMatrixf(&object->getMatrix()[0]);
		shadow_volume.initWedgeRendering();

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_STENCIL_TEST);

		glDepthMask(GL_FALSE);

		unsigned int numwedges = shadow_volume.getEdgeCount();

		if (zfail) {
			glDepthFunc(GL_GEQUAL);
			glFrontFace(GL_CW);
			render_sub_luminance_zfail(numwedges);
			render_add_luminance_zfail(numwedges);
			glDepthFunc(GL_LESS);
			glFrontFace(GL_CCW);
		}
		else {
			render_sub_luminance_zpass(numwedges);
			render_add_luminance_zpass(numwedges);
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);

		shadow_volume.deInitWedgeRendering();
	glPopMatrix();
}

void World::render_shadow(Object *object, Light *light, bool clear)
{
	if (clear) {
		glDrawBuffer(GL_FRONT);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
			(*it)->render();
		}
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDrawBuffer(GL_BACK);
		glClearColor(7.0f / 255.0f, 7.0f / 255.0f, 7.0f / 255.0f, 7.0f / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
			(*it)->render();
		}
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	if (object->getShadowMode() != Object::NoShadow) {
		shadow_volume.build(object, light);	
		if (zfail)
			render_umbra_zfail(object);
		else
			render_umbra_zpass(object);

		if ((object->getShadowMode() == Object::SoftShadow) && (GetAsyncKeyState('V') & 0x8000) == 0)
			render_penumbra(object, light);
	}
}

