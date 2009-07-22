/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "ModelManager.hpp"

using namespace bleifrei::tex;
using namespace std;

namespace bleifrei {
	namespace mesh {

		ModelManager::ModelManager(TextureManager &texturemanager)
			: texturemanager(texturemanager)
		{
		}

		ModelManager::~ModelManager(void)
		{
			map<Mesh *, Model>::iterator it;
			for (it = models.begin(); it != models.end(); it++) {
				it->second.destroy();
			}
		}

		Model ModelManager::load(Mesh *mesh, bool transparency)
		{
			Model model;

			map<Mesh *, Model>::iterator it = models.find(mesh);

			if (it != models.end()) return it->second;

			model = Model(texturemanager, *mesh, transparency);
			models.insert(map<Mesh *, Model>::value_type(mesh, model));

			return model;
		}

	}
}