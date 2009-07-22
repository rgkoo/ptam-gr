/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_io_BitInStream_hpp_
#define _include_bleifrei_io_BitInStream_hpp_


// includes
#include <iostream>

namespace bleifrei {
	namespace io {

		class BitInStream {
		public:
			BitInStream(std::istream &in) : in(in), pos(8) {}
			BitInStream &get(bool &value);
			void byteAlign(void)					{ pos = 8;				}
			operator bool(void)						{ return in.good();		}
		private:
			std::istream &in;
			int pos;
			char buffer;
		};

	}
}


#endif //_include_bleifrei_io_BitInStream_hpp_
