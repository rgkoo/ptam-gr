/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_TGAImage_hpp
#define _include_TGAImage_hpp 


// includes
#include <iostream>

#include "Image.hpp"

namespace bleifrei {
	namespace img {

		/** An extension of the Image class that handles the Targa TrueVision format.
		 * @author Ulf Borgenstam
		 */
		class TGAImage : public Image {
		public:
			TGAImage(std::istream &is);

			void save(std::ostream &os, Compression compression = RawCompression);
		};

	}
}

#endif//_include_TGAImage_hpp
