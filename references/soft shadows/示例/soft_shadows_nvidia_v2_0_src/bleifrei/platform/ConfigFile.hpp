/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_ConfigFile_hpp_
#define _include_ConfigFile_hpp_


// includes
#include <string>

namespace bleifrei {
	namespace platform {


		class ConfigFile {
		public:
			ConfigFile(const char *filename);
			~ConfigFile();

			void selectSection(const char *caption);
			void selectSection(const char *caption, unsigned int index);

			void get(const char *key, std::string &str);
			void get(const char *key, int &val);
			void get(const char *key, float &val);
			void get(const char *key, double &val);
			void get(const char *key, bool &val);
			void get(const char *key, int index, std::string &str);
			void get(const char *key, int index, int &val);
			void get(const char *key, int index, float &val);
			void get(const char *key, int index, double &val);
			void get(const char *key, int index, bool &val);

		protected:
			std::string filename;
			std::string section;
		};

	}
}

#endif //_include_ConfigFile_hpp_