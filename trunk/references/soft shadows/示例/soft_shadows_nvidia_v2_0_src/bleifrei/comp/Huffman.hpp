/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _bleifrei_comp_Huffman_hpp_
#define _bleifrei_comp_Huffman_hpp_


// includes
#include <iostream>

namespace bleifrei {
	namespace comp {
		namespace huffman {
			void compress(std::istream &in, std::ostream &out, int numbytes);
			void decompress(std::istream &in, std::ostream &out);
		}

	}
}

#endif //_bleifrei_comp_Huffman_hpp_
