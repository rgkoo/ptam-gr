/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_console_hpp_
#define _include_bleifrei_console_hpp_ 

// includes
#include <string>
#include "../gfx/Font.hpp"

namespace bleifrei {
	namespace io {
		namespace console {

			int getLineCount(void);

			void printf(float r, float g, float b, const char *str, ...);
			void print(float r, float g, float b, const std::string &str);

			void setFont(gfx::Font *font);

			void render(int first, int count);

			void saveLog(const std::string &filename);
		}
	}
}

#endif //_include_bleifrei_console_hpp_
