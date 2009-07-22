/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "BitOutStream.hpp"

namespace bleifrei {
	namespace io {

		BitOutStream &BitOutStream::put(bool value)
		{
			buffer >>= 1;
			if (value) buffer |= 0x80;
			pos++;

			if (pos == 8) {
				out.put(buffer);
				buffer = 0;
				pos = 0; 
			}
			return *this;
		}

		BitOutStream &BitOutStream::flush(void)
		{
			if (pos != 0) {
				buffer >>= 8 - pos;
				out.put(buffer);
				buffer = 0;
				pos = 0; 
			}
			return *this;
		}

	}
}
