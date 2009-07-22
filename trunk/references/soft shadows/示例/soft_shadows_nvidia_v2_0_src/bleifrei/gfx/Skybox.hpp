/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_Skybox_hpp_
#define _include_Skybox_hpp_


// includes
#include <string>

#include "../mesh/Model.hpp"
#include "../tex/TextureManager.hpp"

namespace bleifrei {
	namespace gfx {

		class Skybox {
		public:
			Skybox(void);
			~Skybox(void);

			void load(bleifrei::tex::TextureManager &tm, const std::string file0, const std::string file1, const std::string file2, const std::string file3, const std::string file4, int width = 100, int height = 100, int length = 100);
			void render(void);

		protected:
			mesh::Model model;
			tex::Texture2 texture[5];
		};

	}
}

#endif // _include_Skybox_hpp_