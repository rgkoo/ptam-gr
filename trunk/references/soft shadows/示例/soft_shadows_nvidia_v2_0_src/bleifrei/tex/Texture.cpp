/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include <iostream>
#include "../img/TGAImage.hpp"
#include "../io/console.hpp"
#include "../io/io.hpp"
#include "../platform/types.hpp"

#include "Texture.hpp"

using namespace bleifrei;
using namespace bleifrei::gfx;
using namespace bleifrei::img;
using namespace bleifrei::io;
using namespace bleifrei::platform;
using namespace std;

namespace bleifrei {
	namespace tex {

		GLint minFilter = GL_LINEAR_MIPMAP_LINEAR;
		GLint magFilter = GL_LINEAR;
		float anisotropy = 1.0f;

		Texture1::Texture1(istream &is, Type type) {

			try {
				TGAImage image(is);
				load(image, type);
			}
			catch (img::out_of_memory_exception) {
			}
			catch (img::stream_exception) {
			}
			catch (img::file_format_exception) {
			}
		}

		Texture1::Texture1(Image &image, Type type)
		{
			load(image, type);
		}

		void Texture1::load(Image &image, Type type)
		{
			glGenTextures(1, &id);
			bind();
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, magFilter);
			if (GL_EXT_texture_filter_anisotropic_initialized) {
				glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
			}

			switch (image.getBitDepth()) {
			case 8 :
				gluBuild1DMipmaps(GL_TEXTURE_1D, 1, image.getWidth(), type == AlphaMap ? GL_ALPHA : GL_LUMINANCE, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 24 :
				gluBuild1DMipmaps(GL_TEXTURE_1D, 3, image.getWidth(), GL_RGB, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 32 :
				//gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGBA8, image.getWidth(), GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, image.getWidth(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
				break;
			}
		}

		Texture2::Texture2(istream &is, Type type) {

			try {
				TGAImage image(is);
				load(image, type);
			}
			catch (img::out_of_memory_exception) {
			}
			catch (img::stream_exception) {
			}
			catch (img::file_format_exception) {
			}
		}

		Texture2::Texture2(Image &image, Type type)
		{
			load(image, type);
		}

		void Texture2::create(void)
		{
			glGenTextures(1, &id);
			bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

			if (GL_EXT_texture_filter_anisotropic_initialized) {
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
			}
		}

		void Texture2::load(Image &image, Type type)
		{
			create();

			switch (image.getBitDepth()) {
			case 8 :
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, image.getWidth(), image.getHeight(), type == AlphaMap ? GL_ALPHA : GL_LUMINANCE, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 16 :
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE16, image.getWidth(), image.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, image.getData());
				//gluBuild2DMipmaps(GL_TEXTURE_2D, 1, image.getWidth(), image.getHeight(), GL_LUMINANCE16, GL_UNSIGNED_SHORT, image.getData());
				break;
			case 24 :
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image.getWidth(), image.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 32 :
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, image.getWidth(), image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
				break;
			}
		}

		TextureRect::TextureRect(istream &is, Type type) {

			try {
				TGAImage image(is);
				load(image, type);
			}
			catch (img::out_of_memory_exception) {
			}
			catch (img::stream_exception) {
			}
			catch (img::file_format_exception) {
			}
		}

		TextureRect::TextureRect(Image &image, Type type)
		{
			load(image, type);
		}
		
		void TextureRect::create(void)
		{
			glGenTextures(1, &id);
			bind();
			glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, magFilter);
			if (GL_EXT_texture_filter_anisotropic_initialized) {
				glTexParameterfv(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
			}
		}

		void TextureRect::load(Image &image, Type type)
		{
			create();

			switch (image.getBitDepth()) {
			case 8 :
				glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 1, image.getWidth(), image.getHeight(), 0, type == AlphaMap ? GL_ALPHA : GL_LUMINANCE, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 16 :
				glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 1, image.getWidth(), image.getHeight(), 0, GL_LUMINANCE16, GL_UNSIGNED_SHORT, image.getData());
				break;
			case 24 :
				glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 3, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
				break;
			case 32 :
				glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, 4, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
				break;
			}
		}

		TextureCube::TextureCube(Image &neg_x, Image &pos_x,
							Image &neg_y, Image &pos_y,
							Image &neg_z, Image &pos_z, Type type)
		{
			glGenTextures(1, &id);
			bind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
			if (GL_EXT_texture_filter_anisotropic_initialized) {
				glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
			}

			if (neg_x.getBitDepth() != pos_x.getBitDepth() ||
				pos_x.getBitDepth() != neg_y.getBitDepth() ||
				neg_y.getBitDepth() != pos_y.getBitDepth() ||
				pos_y.getBitDepth() != neg_z.getBitDepth() ||
				neg_z.getBitDepth() != pos_z.getBitDepth())
			{
				console::print(1.0f, 0.0f, 0.0f, "Cube map sides must have same bit depth!");
				return;
			}

			switch (neg_x.getBitDepth()) {
			case 8 :
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_LUMINANCE, neg_x.getWidth(), neg_x.getHeight(), 0, 1, GL_UNSIGNED_BYTE, neg_x.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_LUMINANCE, pos_x.getWidth(), pos_x.getHeight(), 0, 1, GL_UNSIGNED_BYTE, pos_x.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_LUMINANCE, neg_y.getWidth(), neg_y.getHeight(), 0, 1, GL_UNSIGNED_BYTE, neg_y.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_LUMINANCE, pos_y.getWidth(), pos_y.getHeight(), 0, 1, GL_UNSIGNED_BYTE, pos_y.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_LUMINANCE, neg_z.getWidth(), neg_z.getHeight(), 0, 1, GL_UNSIGNED_BYTE, neg_z.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_LUMINANCE, pos_z.getWidth(), pos_z.getHeight(), 0, 1, GL_UNSIGNED_BYTE, pos_z.getData());
				break;
			case 16 :
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_LUMINANCE16, neg_x.getWidth(), neg_x.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, neg_x.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_LUMINANCE16, pos_x.getWidth(), pos_x.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, pos_x.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_LUMINANCE16, neg_y.getWidth(), neg_y.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, neg_y.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_LUMINANCE16, pos_y.getWidth(), pos_y.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, pos_y.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_LUMINANCE16, neg_z.getWidth(), neg_z.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, neg_z.getData());
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_LUMINANCE16, pos_z.getWidth(), pos_z.getHeight(), 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, pos_z.getData());
				break;
			case 24 :
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 3, neg_x.getWidth(), neg_x.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, neg_x.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 3, pos_x.getWidth(), pos_x.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pos_x.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 3, neg_y.getWidth(), neg_y.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, neg_y.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 3, pos_y.getWidth(), pos_y.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pos_y.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 3, neg_z.getWidth(), neg_z.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, neg_z.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 3, pos_z.getWidth(), pos_z.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pos_z.getData());
				break;
			case 32 :
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 4, neg_x.getWidth(), neg_x.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, neg_x.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 4, pos_x.getWidth(), pos_x.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pos_x.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 4, neg_y.getWidth(), neg_y.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, neg_y.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 4, pos_y.getWidth(), pos_y.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pos_y.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 4, neg_z.getWidth(), neg_z.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, neg_z.getData());
				gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 4, pos_z.getWidth(), pos_z.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pos_z.getData());
				break;
			}
		}

	}
}