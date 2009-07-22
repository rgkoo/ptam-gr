/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_ModelManager_hpp_
#define _include_bleifrei_ModelManager_hpp_

// includes
#include <map>

#include "../tex/TextureManager.hpp"
#include "Mesh.hpp"
#include "Model.hpp"

namespace bleifrei {
	namespace mesh {

		class ModelManager {
		public:
			ModelManager(tex::TextureManager &texturemanager);
			~ModelManager(void);

			Model load(Mesh *mesh, bool transparency = true);

		protected:
			tex::TextureManager &texturemanager;
			std::map<Mesh *, Model> models;
		};

	}
}

#endif //_include_bleifrei_ModelManager_hpp_