/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_Image_hpp
#define _include_Image_hpp 


// includes
#include <iostream>

#include "../platform/types.hpp"

namespace bleifrei {
	namespace img {

		class stream_exception {};
		class out_of_memory_exception {};
		class file_format_exception {};

		/** An image class.
		 * Copyright 2003 (c) \n
		 * A class that can hold an image of a specific width, height and bit depth. The
		 * image can be loaded from/saved to file in both compressed and uncompressed form.
		 * The method copy can be used to copy the image data from another image and if
		 * necessary convert it to a new bit depth. \n
		 * The methods getPixel and putPixel allows for easy manipulation of specific pixels
		 * inside the image
		 * @author Ulf Borgenstam
		 */
		class Image {
		public:
			// types
			enum Compression { RawCompression, RLECompression };

			// constructors
			Image();
			Image(std::istream &is);
			Image(int bpp);
			Image(int width, int height, int bpp);
			Image(std::istream &is, int width, int height, int bpp);

			// destructor
			~Image();

			// public methods
			void create(int width, int height, int bpp);
			void destroy(void);
			void flip(void);
			void putPixel(int x, int y, platform::byte r, platform::byte g, platform::byte b, platform::byte alpha = 0);
			void getPixel(int x, int y, platform::byte &r, platform::byte &g, platform::byte &b, platform::byte &alpha);
			void copy(Image &image, int bpp);

			// stream operations
			void decodeRLE(std::istream &is, int numbytes);
			void encodeRLE(std::ostream &os);
			void save(std::ostream &os, Compression compression = RawCompression);
			void load(std::istream &is);

			// public constant methods
			int getWidth(void) const					{ return width;			}
			int getHeight(void) const					{ return height;		}
			int getBitDepth(void) const					{ return bpp;			}
			int getPixelWidth(void) const				{ return pixelwidth;	}
			int getBytesPerLine(void) const				{ return bytesperline;	}
			platform::byte *getData(void)				{ return imagedata;		}

		protected:

			void convertBGRtoRGB32();
			void convertBGRtoRGB24();
			void convertBGRtoRGB();

			// attributes
			platform::byte *imagedata;
			int width, height;
			int bytesperline;
			int pixelwidth;
			int bpp;
		};

	}
}

#endif//_include_Image_hpp
