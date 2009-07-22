/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <memory>

#include "../io/io.hpp"

#include "Image.hpp"

using namespace bleifrei::io;
using namespace bleifrei::platform;
using namespace std;

namespace bleifrei {
	namespace img {


		#pragma pack(push, 1)

		// Image header
		struct ImageHeader {
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char palette;
			unsigned char compression;
			unsigned char reserved;
		};

		#pragma pack(pop)

		inline static word rgb16bit555(byte r, byte g, byte b)
		{
			return (b >> 3) + ((g & 0xF8) << 2) + ((r & 0xF8) << 7);
		}

		inline static word rgb16bit565(byte r, byte g, byte b)
		{
			return (b >> 3) + ((g & 0xF8) << 3) + ((r & 0xF8) << 8);
		}

		/** Returns the rgb components of the pixel at src.
		 * The color components of the pixel pointed to by \c src are extracted and converted
		 * into byte values. \c bpp tells the function how to decode the pixel data.
		 * @param "byte *src" byte pointer to source pixel
		 * @param "int bpp" bitdepth of pixel
		 * @param "byte &r" Reference to byte where the red component is to be stored
		 * @param "byte &g" Green component
		 * @param "byte &b" Blue component
		 * @param "byte &alpha" Alpha component
		 * @returns \c true if the bit depth is valid, \c false otherwise
		 */
		inline static bool getRGB(byte *src, int bpp, byte &r, byte &g, byte &b, byte &alpha) {
			word v;

			// Decode bit depth
			switch (bpp) {
			case 15:
				// Convert color from 15 bits
				v = *((unsigned short *)src);
				r = byte((v & 0x7C00) >> 10);
				g = byte((v & 0x03E0) >> 5);
				b = byte(v & 0x001F);
				break;
			case 16:
				// Convert color from 16 bits
				v = *((unsigned short *)src);
				r = byte((v & 0xF800) >> 11);
				g = byte((v & 0x07E0) >> 5);
				b = byte(v & 0x001F);
				break;
			case 32:
				// Convert color from 32 bits (RGBA)
				alpha = src[3];
			case 24:
				// Convert color from 24 bits (RGB)
				r = src[2];
				g = src[1];
				b = src[0];
				break;
			default:
				// Unnkown bit depth, return false to indicate failure
				return false;
				break;
			}
			return true;
		}

		/** Sets pixel at dest to RGBA.
		 * The arguments r, g, b and alpha are converted to the pixel format specified by \c bpp
		 * and stored at the location pointed to by \c dest.
		 * The color components of the pixel pointed to by \c src are extracted and converted
		 * into byte values. \c bpp tells the function how to decode the pixel data.
		 * @param "byte *dest" byte pointer to destination pixel
		 * @param "int bpp" Bit depth of pixel
		 * @param "byte r" Red component
		 * @param "byte g" Green component
		 * @param "byte b" Blue component
		 * @param "byte alpha" Alpha component
		 * @returns \c true if the bit depth is valid, \c false otherwise
		 */
		inline static bool putRGB(byte *dest, int bpp, byte r, byte g, byte b, byte alpha) {

			// Convert 32 bit data to the correct format
			switch (bpp) {
			case 15:
				// 15 bit 555 format
				*((word *)dest) = rgb16bit555(r,g,b);
				break;
			case 16:
				// 16 bit 565 format
				*((word *)dest) = rgb16bit565(r,g,b);
				break;
			case 32:
				// 32 bits
				dest[3] = alpha;
			case 24:
				// 24 bits
				dest[2] = r;
				dest[1] = g;
				dest[0] = b;
				break;
			default:
				// Unknown bit depth, return false to signal failure
				return false;
			}
			return true;
		}

		/** Create new empty image.
		 * @param none
		 */
		Image::Image() : imagedata(0) {
		}

		/** Create image and read it from stream.
		 * @param "istream &is" Source stream
		 */
		Image::Image(istream &is) {
			imagedata = 0;
			load(is);
		}

		/** Create new empty image with specified bit depth.
		 * @param "int bpp" Bit depth
		 */
		Image::Image(int bpp) {
			imagedata = 0;
			this->bpp = bpp;
		}

		/** Create a new image and initialize it to the specified dimensions.
		 * @param "int width" Width of the image
		 * @param "int height" Width of the image
		 * @param "int bpp" Bit depth fo image
		 */
		Image::Image(int width, int height, int bpp) {
			imagedata = 0;
			create(width, height, bpp);
		}

		/** Create new image and red RAW data from a stream.
		 * The parameters width, height and bpp are used to create a new image of the
		 * specified dimensions and to calculate how much data should be read from the stream.
		 * read raw image data from the stream.
		 * @param "istream &is" Source stream
		 * @param "int width" Width of the image
		 * @param "int height" Width of the image
		 * @param "int bpp" Bit depth fo image
		 * @throws stream_exception
		 */
		Image::Image(istream &is, int width, int height, int bpp) {
			imagedata = 0;

			// Create new image
			create(width, height, bpp);

			// Read image data
			is.read(((char *)(imagedata)), bytesperline * height);

			// If error throw exception
			if (!is) throw stream_exception();
		}

		/** Initialize image data.
		 * Reset the Image to the specified dimensions.
		 * @param "int width" Width of the image
		 * @param "int height" Width of the image
		 * @param "int bpp" Bit depth fo image
		 * @throws OutOfMemoryException
		 * @returns void
		 */
		void Image::create(int width, int height, int bpp) {
			destroy();
			imagedata = 0;

			// Set attributes
			this->width = width;
			this->height = height;
			this->bpp = bpp;

			// Calculate unsupplied attributes
			pixelwidth = (bpp + 7) / 8;			// Width in bytes
			bytesperline = pixelwidth * width;

			// Allocate memory
			imagedata = new byte[bytesperline * height];
			if (!imagedata) throw out_of_memory_exception();
		}

		/** Destroys the image.
		 */
		Image::~Image() {
			destroy();
		}

		/** Destroys the image.
		 * Deallocates memory
		 * @param none
		 * @returns void
		 */
		void Image::destroy(void) {
			if (imagedata) delete[] imagedata;
		}

		/** Flips the bitmap upside down.
		 * @param none
		 * @throws out_of_memory_exception
		 * @returns void
		 */
		void Image::flip() {

			// Create temporary storage for one line
			byte *temp = new byte[bytesperline];

			// throw Exception if not enough memory
			if (!temp) throw out_of_memory_exception();

			// Flip lines one by one
			for (int index = 0; index < height / 2; index++) {

				// First line -> temp
				memcpy(temp, &imagedata[index * bytesperline], bytesperline);

				// Last line -> First line
				memcpy(&imagedata[index * bytesperline], &imagedata[(height - 1 - index) *
					bytesperline], bytesperline);

				// Temp -> Last line
				memcpy(&imagedata[(height - 1 - index) * bytesperline], temp,
					bytesperline);
			}

			// Free temporary memory
			delete[] temp;
		}

		/** Sets pixel at (x,y) to color (r, g, b, alpha)
		 * Converts the color components to the correct pixelformat and saves it at
		 * position (x, y).
		 * @param "int x" X location
		 * @param "int y" Y location
		 * @param "unsigned char r" Red component of color
		 * @param "unsigned char g" Green component of color
		 * @param "unsigned char b" Blue component of color
		 * @param "unsigned char alpha" Alpha component of color (if 32bit)
		 * @returns void
		 */
		void Image::putPixel(int x, int y, byte r, byte g, byte b, byte alpha) {

			// Set destination pointer
			byte *dest = &imagedata[x * pixelwidth + bytesperline * y];

			// Is it an 8 bit image?
			if (bpp == 8) {

				// Greyscale
				*dest = (byte)(((short)r + (short)g + (short)b) / 3);
			}
			// .. otherwise create hi/truecolor pixel data 
			else {
				putRGB(dest, bpp, r, g, b, alpha);
			}
		}

		/** Return RGBA from pixel at (x,y).
		 * Converts the color components from the pixel at position (x,y) to bytes and sets the
		 * values of the references r, g, b and alpha.
		 * @param "int x" X location
		 * @param "int y" Y location
		 * @param "unsigned char &r" Reference to byte where to store red component of color
		 * @param "unsigned char &g" Green component of color
		 * @param "unsigned char &b" Blue component of color
		 * @param "unsigned char alpha" Alpha component of color (if 32bit)
		 * @returns void
		 */
		void Image::getPixel(int x, int y, byte &r, byte &g, byte &b, byte &alpha) {
			
			// Set source pointer
			byte *source = &imagedata[y * bytesperline + x * pixelwidth];

			// If it is an 8 bit image
			if (bpp == 8) {
				r = g = b = *source;
				alpha = 0;
			}
			// .. otherwise get hi/truecolor pixel value from image
			else {
				getRGB(source, bpp, r, g, b, alpha);
			}
		}

		/** Copy image data from one Image to another.
		 * Uses the methods getPixel and putPixel to copy the pixels from one image to another.
		 * @warning Completely destroys old image data
		 * @param int bpp Bit depth of destination image
		 * @throws out_of_memory_exception
		 * @returns void
		 * @author @top
		 */
		void Image::copy(Image &image, int bpp) {
			
			// Temporary storage for color components
			byte r, g, b, alpha;

			// Initialize image to a resolution matching the source resolution and the given bit depth
			create(image.width, image.height, bpp);

			// Copy each pixel (Slow)
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					image.getPixel(x, y, r, g, b, alpha);
					putPixel(x, y, r, g, b, alpha);
				}
			}
		}

		/** Decodes Run-Length Encoded data from a stream.
		 * @param "istream &is" Source stream
		 * @param "int numbytes" Size of RLE data in bytes
		 * @throws stream_exception
		 * @returns void
		 */
		void Image::decodeRLE(istream &is, int numbytes) {
			byte c;

			// While there are still bytes left to read from the stream
			for (int ptr = 0; ptr < numbytes;) {

				if (!is) throw stream_exception();			// Is the stream OK?

				c = is.get();								// Get next byte
				if (c & 0x80) {								// Is it a 'run'?
					byte color[4];
	
					c &= 0x7f;								// Get pixel count
					c++;

					is.read((char *)color, pixelwidth);		// Read color from stream
					
					for (int i = 0; i < c; i++) {			// Write color c times
						for (int j = 0; j < pixelwidth; j++, ptr++) {
							imagedata[ptr] = color[j];
						}
					}
				}
				else {										// Read count pixels
					c++;
					is.read((char *)&imagedata[ptr], c * pixelwidth);
					ptr += c * pixelwidth;
				}
			}
		}

		/** Compare two pixels (Used when encoding RLE data).
		 * @param "byte *p" Pointer to pixel data
		 * @param "int pixelwidth" Width of a pixel in bytes
		 * @returns \c true if the pixels match, \c false otherwise
		 */
		inline static bool comparepixels(byte *p, int pixelwidth) {

			// Test all bytes of pixel
			for (int i = 0; i < pixelwidth; i++) {

				// Does pixel match the next one? If not return false
				if (p[i] != p[i + pixelwidth]) return false;
			}
			return true;
		}

		/** Write a 'run' to a stream (RLE help function).
		 * @param "ostream &os" Destination stream
		 * @param "byte *src" Source pixel
		 * @param "int count" Number of pixels to write
		 * @param "int pixelwidth" Width of a pixel in bytes
		 * @returns void
		 */
		inline static void writeRLE(ostream &os, byte *src, int count, int pixelwidth) {
			os.put((byte)((count - 1) | 0x80));
			os.write((char *)src, pixelwidth);
		}

		/** Copy count pixels from memory to a stream (RLE help function).
		 * @param "ostream &os" Destination stream
		 * @param "byte *src" Byte pointer to source pixels
		 * @param "int count" Number of pixels to write
		 * @param "int pixelwidth" Width of a pixel in bytes
		 * @returns void
		 */	
		inline static void writeRAW(ostream &os, byte *src, int count, int pixelwidth) {
			os.put((byte)(count - 1));
			os.write((char *)src, count * pixelwidth);
		}

		/** Writes RLE data to a stream.
		 * @param "ostream &os" Destination stream
		 * @returns void
		 */
		void Image::encodeRLE(ostream &os) {

			// Pointer to the first pixel to copy
			byte *firstpixel = NULL;

			// Number of pixels in current 'run'
			int c1 = 0;

			// Number of pixels to copy
			int c2 = 0;

			int ptr = 0;
			int size = bytesperline * height - pixelwidth;

			// While there are still pixels to write
			while (ptr < size) {
				int isequal = comparepixels(&imagedata[ptr], pixelwidth);

				// If next pixel equals the last ones and there are less than
				// 127 pixels in this repeat operation
				if (isequal && c1 < 127) {

					// Increase "run" length and pointer
					c1++;
					ptr += pixelwidth;

					// If c2 > 0 there are c2 pixels from a previous copy
					// operation to write to stream
					if (c2) {
						
						// Write any pixels
						writeRAW(os, firstpixel, c2, pixelwidth);

						// Reset copy counter and pointer to first pixel
						firstpixel = NULL;
						c2 = 0;
					}
				}
				// If there are 127 pixels write them to the stream
				else if (c1) {
						writeRLE(os, &imagedata[ptr], c1 + 1, pixelwidth);
						c1 = 0;
						ptr += pixelwidth;
				}
				// 
				else {

					// Start new run
					if (firstpixel == NULL) firstpixel = &imagedata[ptr];

					// Update pointers
					c2++;
					ptr += pixelwidth;

					// Copy 128 pixels to stream
					if (c2 == 128) {
						writeRAW(os, firstpixel, c2, pixelwidth);

						// Reset pointer and "run" length
						firstpixel = NULL;
						c2 = 0;
					}
				}
			}

			// If there are still unwritten pixels when we exit the loop we
			// write them
			if (c1) {
				writeRLE(os, &imagedata[ptr], c1 + 1, pixelwidth);
			}
			else if (c2) {
				writeRAW(os, firstpixel, c2 + 1, pixelwidth);
			}
		}

		/** Writes an image to an ostream, compressed or uncompressed.
		 * @param "ImageCompression compression" Compression algorithm
		 * @returns void
		 */
		void Image::save(ostream &os, Compression compression) {
			FileId id;
			ImageHeader header;

			if (compression > RLECompression || compression < RawCompression) {
				compression = RawCompression;			// If unknown compression, force RAW
			}

			// Fill file header
			id = string2id("IMG1");

			header.height = height;						// Image width
			header.width = width;						// Image height
			header.bpp = bpp;							// Image bit depth
			header.palette = 0;							// Image does not have palette
			header.compression = compression;			// Image compression format
			header.reserved = 0;						// Padding

			os.write((char *)&id, sizeof(id));			// Write file header

			os.write((char *)&header, sizeof(header));	// Write image header

			switch (compression) {						// Select compression
			case RawCompression :
				os.write((char *)imagedata, bytesperline * height);
				break;
			case RLECompression :
				encodeRLE(os);
				break;
			}
		}

		/** Reads an image from an istream.
		 * @param "istream &is" Source stream
		 * @throws file_format_exception, out_of_memory_exception
		 * @returns void
		 */
		void Image::load(istream &is) {
			FileId id;
			ImageHeader header;

			// Read file header
			is.read((char *)&id, sizeof(id));

			// Make sure it is an imagefile
			if (id != string2id("IMG1")) throw file_format_exception();
			
			// Read image header
			is.read((char *)&header, sizeof(header));

			// Read palette if there is one
			if (header.palette) {
				throw file_format_exception();
			}

			// Initialize image    
			create(header.width, header.height, header.bpp);

			// Decode imagedata
			switch (header.compression) {
			case RawCompression:
				is.read((char *)imagedata, bytesperline * height);
				break;
			case RLECompression:
				decodeRLE(is, bytesperline * height);
				break;
			default:
				throw file_format_exception();
			}
		}

		/** Convert image data from BGR to RGB (32 bit).
		 * @param none	
		 * @returns void
		 * @author @top
		 */
		void Image::convertBGRtoRGB32() {
			byte *maxPtr = imagedata + bytesperline * height;
			for (byte *ptr = imagedata; ptr < maxPtr; ptr += 4) {
				ptr[2] ^= ptr[0];
				ptr[0] ^= ptr[2];
				ptr[2] ^= ptr[0];
			}
		}

		/** Convert image data from BGR to RGB (24 bit).
		 * @param none	
		 * @returns void
		 * @author @top
		 */
		void Image::convertBGRtoRGB24() {
			byte *maxPtr = imagedata + bytesperline * height;
			for (byte *ptr = imagedata; ptr < maxPtr; ptr += 3) {
				ptr[2] ^= ptr[0];
				ptr[0] ^= ptr[2];
				ptr[2] ^= ptr[0];
			}
		}

		/** Convert image data from BGR to RGB (all formats).
		 * @param none	
		 * @returns void
		 * @author @top
		 */
		void Image::convertBGRtoRGB() {
			if (bpp == 8) return;
			for (int i = 0; i < getHeight(); i++) {
				for (int j = 0; j < getWidth(); j++) {
					byte r, g, b, alpha;
					getPixel(j, i, r, g, b, alpha);
					putPixel(j, i, b, g, r, alpha);
				}
			}
		}

	}
}
