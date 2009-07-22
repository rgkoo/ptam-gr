/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_MeshManager_hpp_
#define _include_bleifrei_MeshManager_hpp_

// includes
#include <string>
#include <map>

#include "Mesh.hpp"

namespace bleifrei {
	namespace mesh {

		class MeshManager {
		public:
			MeshManager(const std::string &directory = "");
			~MeshManager(void);

			Mesh *load(const std::string &filename, bool calculateEdges = false);

		protected:
			std::string directory;
			std::map<std::string, Mesh *> meshes;
		};

	}
}

#endif //_include_bleifrei_TextureManager_hpp_