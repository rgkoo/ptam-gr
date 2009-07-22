/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <fstream>

#include "MeshManager.hpp"

using namespace std;

namespace bleifrei {
	namespace mesh {

		MeshManager::MeshManager(const string &directory)
		{
			this->directory = directory;
		}

		MeshManager::~MeshManager(void)
		{
			map<string, Mesh *>::iterator it;
			for (it = meshes.begin(); it != meshes.end(); it++) {
				delete it->second;
			}
		}

		Mesh *MeshManager::load(const string &filename, bool calculateEdges)
		{
			Mesh *mesh;

			map<string, Mesh *>::iterator it = meshes.find(filename);

			if (it != meshes.end()) return it->second;

			mesh = new Mesh(ifstream((directory + filename).c_str(), ios::binary), calculateEdges);		
			meshes.insert(map<string, Mesh *>::value_type(filename, mesh));

			return mesh;
		}

	}
}