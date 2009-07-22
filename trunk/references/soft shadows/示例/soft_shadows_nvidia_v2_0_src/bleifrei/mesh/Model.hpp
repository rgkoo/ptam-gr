/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Model_hpp_
#define _include_bleifrei_Model_hpp_ 


// includes
#include "../gfx/opengl.hpp"
#include "../platform/types.hpp"
#include "../tex/TextureManager.hpp"

namespace bleifrei {
	namespace mesh {

		class Mesh;

		class Model {
		public:
			Model(void)					{}
			Model(platform::dword id)	{ this->id = id;			}
			Model(const Model &m)		{ id = m.id;				}
			Model(bleifrei::tex::TextureManager &tm, Mesh &m, bool transparency = true);
			
			Model &operator =(Model &m)	{ id = m.id; return *this;	}

			void destroy(void)			{ glDeleteLists(id, 1);		}
			void render(void)			{ glCallList(id);			}
		protected:
			GLuint id;
		};

	}
}

#endif //_include_bleifrei_Model_hpp_
