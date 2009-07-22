/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "Mesh.hpp"
#include "Model.hpp"
#include "../tex/TextureManager.hpp"

using namespace bleifrei::gfx;
using namespace bleifrei::tex;
using namespace std;

namespace bleifrei {
	namespace mesh {

		Model::Model(TextureManager &tm, Mesh &m, bool transparency)
		{
			id = glGenLists(1);

			vector<Material> &materials = m.get_materials();

			vector<Texture2> textures;
			vector<std::string>::iterator tex_it;
			for (tex_it = m.get_textures().begin(); tex_it != m.get_textures().end(); ++tex_it) {
				textures.push_back(tm.load(*tex_it));
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, &m.get_vertices()[0]);
			glNormalPointer(GL_FLOAT, 0, &m.get_normals()[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &m.get_texcoords()[0]);

			glNewList(id, GL_COMPILE);
				int material = -1, texture = -1;

				vector<Triangle>::iterator it;
				for (it = m.get_triangles().begin(); it != m.get_triangles().end(); it++) {

					if (textures.size() && it->texture != texture) textures[texture = it->texture].bind();
					if (materials.size() && it->material != material) materials[material = it->material].bind();
					
					if (transparency && materials[it->material].transparent == true) {
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					}

					glBegin(GL_TRIANGLES);
						glArrayElement(it->v[0]);
						glArrayElement(it->v[1]);
						glArrayElement(it->v[2]);
					glEnd();

					if (transparency && materials[it->material].transparent == true) {
						glDisable(GL_BLEND);
					}
				}

			glEndList();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

	}
}