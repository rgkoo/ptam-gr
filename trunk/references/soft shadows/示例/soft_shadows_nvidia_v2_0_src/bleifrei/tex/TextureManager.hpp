/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_TextureManager_hpp_
#define _include_bleifrei_TextureManager_hpp_

// includes
#include <string>
#include <map>

#include "Texture.hpp"

namespace bleifrei {
	namespace tex {

		class TextureManager {
		public:
			TextureManager(const std::string &directory = "");
			~TextureManager(void);

			Texture2 load(const std::string &filename);

		protected:
			std::string directory;
			std::map<std::string, Texture2> textures;
		};

	}
}

#endif //_include_bleifrei_TextureManager_hpp_