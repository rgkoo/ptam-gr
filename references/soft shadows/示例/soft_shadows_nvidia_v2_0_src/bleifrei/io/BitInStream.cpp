/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "BitInStream.hpp"

using namespace std;

namespace bleifrei {
	namespace io {

		BitInStream &BitInStream::get(bool &value)
		{
			if (pos == 8) {
				in.get(buffer);
				pos = 0;
			}
			value = buffer & 0x01;
			buffer >>= 1;
			++pos;
			return *this;
		}

	}
}
