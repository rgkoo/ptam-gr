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

#include "TextureManager.hpp"

using namespace std;

namespace bleifrei {
	namespace tex {

		TextureManager::TextureManager(const string &directory)
		{
			this->directory = directory;
		}

		TextureManager::~TextureManager(void)
		{
			map<string, Texture2>::iterator it;
			for (it = textures.begin(); it != textures.end(); it++) {
				it->second.destroy();
			}
		}

		Texture2 TextureManager::load(const string &filename)
		{
			Texture2 texture;

			map<string, Texture2>::iterator it = textures.find(filename);

			if (it != textures.end()) return it->second;	// Return texture if it is already loaded

			texture = Texture2(ifstream((directory + filename).c_str(), ios::binary));		

			textures.insert(map<string, Texture2>::value_type(filename, texture));

			return texture;
		}

	}
}