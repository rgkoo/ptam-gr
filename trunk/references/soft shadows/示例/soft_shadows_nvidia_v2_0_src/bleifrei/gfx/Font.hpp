/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_Font_hpp_
#define _include_Font_hpp_


// includes
#include <string>

#include "../gfx/opengl.hpp"
#include "../io/ConfigFile.hpp"
#include "../tex/TextureManager.hpp"

namespace bleifrei {
	namespace gfx {

		class Font {
		public:
			Font(io::ConfigFile &config_file, tex::TextureManager &tm);
			~Font(void);

			// methods
			void printf(GLint x, GLint y, GLfloat scale, const char *str, ...);
			void print(GLint x, GLint y, GLfloat scale, int numchars, const char *str);
			void print(GLint x, GLint y, GLfloat scale, const std::string &str);

			int getMaxChars(const char *str, int numchars, int maxWidth);
			int getMaxChars(const std::string &str, int maxWidth);
			int getWidth(const char *str, int numchars);
			int getWidth(const std::string &str);
			int getHeight() const						{ return height;		}
			const std::string getName(void) const		{ return name;			}

			void bindTexture()							{ texture.bind();		}
		protected:

			// attrubutes
			std::string name;

			tex::Texture2 texture;

			GLuint charBase;
			unsigned char width[256];
			int height;
		};

		inline void Font::print(GLint x, GLint y, GLfloat scale, const std::string &str) {
			print(x, y, scale, (int)str.length(), str.c_str());
		}

		inline int Font::getMaxChars(const std::string &str, int maxWidth) {
			return getMaxChars(str.c_str(), (int)str.length(), maxWidth);
		}

		inline int Font::getWidth(const std::string &str) {
			return getWidth(str.c_str(), (int)str.length());
		}

	}
}


#endif //_include_glFont_hpp_
