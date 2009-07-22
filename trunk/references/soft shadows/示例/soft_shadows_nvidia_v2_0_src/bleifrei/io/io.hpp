/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_io_hpp_
#define _include_bleifrei_io_hpp_ 


// includes
#include <iostream>

#include "../platform/types.hpp"

namespace bleifrei {
	namespace io {

		#pragma pack(push, 1)

		typedef platform::dword FileId;

		struct FileVersion {
			platform::word major;
			platform::word minor;
		};

		#pragma pack(pop)

		template<class T>
		inline void get(std::istream &is, T &t)
		{
			is.read((char *)&t, sizeof(t));
		}

		template<class T>
		inline void put(std::ostream &is, T &t)
		{
			is.write((char *)&t, sizeof(t));
		}

		inline FileId string2id(const char *str)
		{
			return *((FileId *)str);
		}

	}
}


#endif //_include_bleifrei_io_hpp_ 
