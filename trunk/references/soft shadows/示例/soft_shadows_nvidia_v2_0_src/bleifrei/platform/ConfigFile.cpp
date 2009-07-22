/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "ConfigFile.hpp"

using namespace std;

namespace bleifrei {
	namespace platform {
		
		ConfigFile::ConfigFile(const char *filename) {
			char buffer[256];
		    GetCurrentDirectory(256, buffer);
		    this->filename = buffer;
			this->filename += "\\";
			this->filename += filename;
		}

		ConfigFile::~ConfigFile() {
		}

		void ConfigFile::selectSection(const char *caption) {
			section = caption;
		}

		void ConfigFile::selectSection(const char *caption, unsigned int index) {
			char buffer[20];

			section = caption;
			section += itoa(index, buffer, 10);
		}

		void ConfigFile::get(const char *key, string &str) {
			char buffer[256];

			GetPrivateProfileString(section.c_str(), key, "", buffer, 256, filename.c_str());
			str = buffer;
		}

		void ConfigFile::get(const char *key, int &val) {
			val = GetPrivateProfileInt(section.c_str(), key, 0, filename.c_str());
		}

		void ConfigFile::get(const char *key, float &val) {
			char buffer[256];

			GetPrivateProfileString(section.c_str(), key, "", buffer, 256, filename.c_str());
			val = (float)atof(buffer);
		}

		void ConfigFile::get(const char *key, double &val) {
			char buffer[256];

			GetPrivateProfileString(section.c_str(), key, "", buffer, 256, filename.c_str());
			val = atof(buffer);
		}

		void ConfigFile::get(const char *key, bool &val) {
			string temp;
			get(key, temp);
			val = (temp == "true");
		}

		void ConfigFile::get(const char *key, int index, string &str) {
			char buffer[20];

			string s = key;
			s += itoa(index, buffer, 10);
			get(s.c_str(), str);
		}

		void ConfigFile::get(const char *key, int index, int &val) {
			char buffer[20];

			string s = key;
			s += itoa(index, buffer, 10);
			get(s.c_str(), val);
		}

		void ConfigFile::get(const char *key, int index, float &val) {
			char buffer[20];

			string s = key;
			s += itoa(index, buffer, 10);
			get(s.c_str(), val);
		}

		void ConfigFile::get(const char *key, int index, double &val) {
			char buffer[20];

			string s = key;
			s += itoa(index, buffer, 10);
			get(s.c_str(), val);
		}

		void ConfigFile::get(const char *key, int index, bool &val) {
			string temp;
			get(key, index, temp);
			val = (temp == "true");
		}

	}
}