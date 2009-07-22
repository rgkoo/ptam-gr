
#ifndef _include_World_hpp_
#define _include_World_hpp_ 


// includes
#include <vector>

#include "bleifrei/gfx/FragmentProgram.hpp"
#include "bleifrei/gfx/RenderTarget.hpp"
#include "bleifrei/gfx/VertexProgram.hpp"
#include "bleifrei/mesh/MeshManager.hpp"
#include "bleifrei/mesh/ModelManager.hpp"
#include "bleifrei/tex/Texture.hpp"
#include "bleifrei/tex/TextureManager.hpp"

#include "Light.hpp"
#include "Object.hpp"
#include "ShadowVolume.hpp"

#include "lookuptextures.hpp"

class World {
public:
	World(void);
	virtual ~World(void);

	virtual unsigned int getLoadSteps(void);

	virtual void render(void);
	virtual void update(void);

	void addLight(Light *light)					{ lights.push_back(light);		}
	void deleteLights(void);

	void addObject(Object *object)				{ objects.push_back(object);	}
	void deleteObjects(void);
protected:
	void render_umbra_zpass(Object *object);
	void render_umbra_zfail(Object *object);
	void render_sub_luminance_zpass(unsigned int numwedges);
	void render_sub_luminance_zfail(unsigned int numwedges);
	void render_add_luminance_zpass(unsigned int numwedges);
	void render_add_luminance_zfail(unsigned int numwedges);
	void render_penumbra(Object *object, Light *light);
	void render_shadow(Object *object, Light *light, bool clear);
	void render(bleifrei::gfx::RenderTarget *render_target, Light *light);

	std::vector<Object *> objects;
	std::vector<Light *> lights;

	bleifrei::mesh::MeshManager mesh_manager;
	bleifrei::mesh::ModelManager model_manager;
	bleifrei::tex::TextureManager texture_manager;

	bleifrei::tex::TextureCube atan2_texture;
	bleifrei::tex::Texture2 sphericalarea_texture;
	bleifrei::tex::Texture2 rectarea_texture;
	bleifrei::tex::Texture1 color2xlookup_texture;
	bleifrei::tex::Texture1 colorlookup_texture;

	bleifrei::tex::TextureRect position_texture;
	bleifrei::tex::TextureRect additive_luminance_texture;
	bleifrei::tex::TextureRect subtractive_luminance_texture;
	bleifrei::tex::TextureRect screen_texture;

	bleifrei::gfx::RenderTarget *position_buffer;
	bleifrei::gfx::RenderTarget *luminance_buffer;
	bleifrei::gfx::RenderTarget *screen_buffer;

	bleifrei::gfx::VertexProgram vp_light;
	bleifrei::gfx::VertexProgram vp_posworld;
	bleifrei::gfx::VertexProgram vp_shadowvolume;
	bleifrei::gfx::VertexProgram vp_wedge;
	bleifrei::gfx::FragmentProgram fp_color;
	bleifrei::gfx::FragmentProgram fp_light;
	bleifrei::gfx::FragmentProgram fp_light_rect;
	bleifrei::gfx::FragmentProgram fp_posworld;
	bleifrei::gfx::FragmentProgram fp_rectwedge;
	bleifrei::gfx::FragmentProgram fp_spherewedge;
	bleifrei::gfx::FragmentProgram fp_texturedwedge;

	ShadowVolume shadow_volume;

	bool zfail;
};


#endif //_include_World_hpp_
