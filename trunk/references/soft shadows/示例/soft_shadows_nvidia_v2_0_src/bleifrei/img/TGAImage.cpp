/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "TGAImage.hpp"

using namespace std;

namespace bleifrei {
	namespace img {

		#pragma pack(push, 1)

		// Truevision Targa
		struct TGAColorMapInfo {
			unsigned short origin;
			unsigned short length;
			unsigned char bpp;
		};

		struct TGAHeader {
			unsigned char idlen;
			unsigned char colType;
			unsigned char imType;
			TGAColorMapInfo colormapinfo;
			unsigned short x;
			unsigned short y;
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char imdesc;
		};

		#pragma pack(pop)

		/** Load a TGA file from a stream.
		 * Interpret data in stream as a Targa file and use it to create a new Image object.
		 * @param "istream &is" Source stream
		 * @throws StreamException
		 * @throws FileFormatException
		 * @throws OutOfMemoryException
		 */
		TGAImage::TGAImage(istream &is)
		{
			TGAHeader tgahead;

			// Read TGA header
			is.read((char *)&tgahead, sizeof(tgahead));

			// Throw exception if error occurred
			if (!is) {
				throw stream_exception();
			}

			// Initialize image
			create(tgahead.width, tgahead.height, tgahead.bpp);

			// Ignore identification field
			if (tgahead.idlen) is.ignore(tgahead.idlen);

			// Does the image have a palette?
			if (tgahead.colType) {
				int palbytes = ((tgahead.colormapinfo.length - tgahead.colormapinfo.origin) * tgahead.colormapinfo.bpp + 7) / 8;
				is.ignore(palbytes);
			}

			if (!is) {
				throw stream_exception();
			}

			// Decode image data
			switch (tgahead.imType) {
			// Uncompressed
			case 1:
			case 2:
			case 3:
				is.read((char *)imagedata, bytesperline * height);
				break;

			// RLE encoded
			case 9:
			case 10:
			case 11:
				decodeRLE(is, bytesperline * height);
				break;

			// Unknown file format
			default:
				throw file_format_exception();
			}

			// Convert image to RGB format
			switch (bpp) {
			case 32:
				convertBGRtoRGB32();
				break;
			case 24:
				convertBGRtoRGB24();
				break;
			default:
				convertBGRtoRGB();
			}
			
			if (!is) {
				throw stream_exception();
			}

			//flip();
		}

		void TGAImage::save(ostream &os, Compression compression)
		{
			TGAHeader tgahead;

			tgahead.width = this->width;
			tgahead.height = this->height;
			tgahead.bpp = this->bpp;
			tgahead.idlen = 0;
			tgahead.colType = 0;
			tgahead.imType = compression == RawCompression ? 2 : 10;

			os.write((char *)&tgahead, sizeof(tgahead));
			//os.write("bleifrei", 8);

			if (compression == RawCompression) {
				os.write((char *)imagedata, bytesperline * height);
			}
			else {
				encodeRLE(os);
			}
		}

	}
}