/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_io_BitOutStream_hpp_
#define _include_bleifrei_io_BitOutStream_hpp_


// includes
#include <iostream>
#include <vector>

namespace bleifrei {
	namespace io {

		class BitOutStream {
		public:
			BitOutStream(std::ostream &out) : out(out), pos(0) {}
			operator bool(void)						{ return out.good();		}
			BitOutStream &put(bool value);
			BitOutStream &flush(void);
		private:
			std::ostream &out;
			unsigned char buffer;
			int pos;
		};

		inline void operator <<(BitOutStream &bos, std::vector<bool> &bits)
		{
			std::vector<bool>::iterator it;
			for (it = bits.begin(); it != bits.end(); ++it) {
				bos.put(*it);
			}
		}

	}
}


#endif //_include_bleifrei_io_BitOutStream_hpp_
