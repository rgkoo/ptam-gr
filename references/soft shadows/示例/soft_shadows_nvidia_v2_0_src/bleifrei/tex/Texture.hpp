/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Texture_hpp_
#define _include_bleifrei_Texture_hpp_ 


// includes
#include <iostream>

#include "../io/io.hpp"
#include "../gfx/opengl.hpp"
#include "../platform/platform.hpp"
#include "../platform/types.hpp"

namespace bleifrei {
	namespace img {
		class Image;
	}
	namespace tex {

		enum Type {
			AlphaMap,
			LuminanceMap
		};

		enum Filter {
			Nearest,
			Bilinear,
			Trilinear
		};

		enum Format {
			LUM		=	GL_LUMINANCE,
			RGB		=	GL_RGB8,
			RGBA	=	GL_RGBA8
		};

		extern GLint minFilter;
		extern GLint magFilter;
		extern float anisotropy;

		void init_multi_texturing(void);
		void init_texture_rectangle(void);

		inline void setMinFilter(Filter filter)
		{
			if (filter == Bilinear) {
				minFilter = GL_LINEAR;
			}
			else if (filter == Trilinear) {
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
			}
			else {
				minFilter = GL_NEAREST;
			}
		}

		inline void setMagFilter(Filter filter)
		{
			if (filter == Bilinear) {
				magFilter = GL_LINEAR;
			}
			else {
				magFilter = GL_NEAREST;
			}
		}

		inline void setAnisotropicFiltering(float level)
		{
			if (bleifrei::gfx::GL_EXT_texture_filter_anisotropic_initialized) {
				float maxlevel;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxlevel);

				anisotropy = (level <= maxlevel) ? level : maxlevel;
			}
		}

		inline void setUnit(int texUnit)
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
		}

		class Texture1 {
		public:
			Texture1(void)				{}
			Texture1(const Texture1 &t)	{ id = t.id;						}
			Texture1(std::istream &is, Type type = LuminanceMap);
			Texture1(img::Image &image, Type type = LuminanceMap);

			void bind(void)				{ glBindTexture(GL_TEXTURE_1D, id);	}

			void destroy(void)			{ glDeleteTextures(1, &id);			}

			static void enable(void)	{ glEnable(GL_TEXTURE_1D);			}
			static void disable(void)	{ glDisable(GL_TEXTURE_1D);			}

		protected:
			void load(img::Image &image, Type type);

			GLuint id;
		};

		class Texture2 {
		public:
			Texture2(void)				{}
			Texture2(const Texture2 &t)	{ id = t.id;						}
			Texture2(std::istream &is, Type type = LuminanceMap);
			Texture2(img::Image &image, Type type = LuminanceMap);

			void bind(void)				{ glBindTexture(GL_TEXTURE_2D, id);	}
			void create(void);
			void destroy(void)			{ glDeleteTextures(1, &id);			}

			static void blit(int x, int y, int dx, int dy, 
						float u = 0.0f, float v = 0.0f, float du = 1.0f, float dv = 1.0f);

			static void enable(void)	{ glEnable(GL_TEXTURE_2D);			}
			static void disable(void)	{ glDisable(GL_TEXTURE_2D);			}

			static void enableSphereMapping(void);
			static void disableSphereMapping(void);

			void copyFromFramebuffer(Format format);

		protected:
			void load(img::Image &image, Type type);

			GLuint id;
		};

		inline void Texture2::copyFromFramebuffer(Format format)
		{
			glCopyTexImage2D(GL_TEXTURE_2D, 0, format, 0, 0, platform::getScreenWidth(), platform::getScreenHeight(), 0);
		}

		inline void Texture2::enableSphereMapping(void)
		{
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		inline void Texture2::disableSphereMapping(void)
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}

		inline void Texture2::blit(int x, int y, int dx, int dy, 
								float u, float v, float du, float dv)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(u, v + dv);
				glVertex2i(x, y);
				glTexCoord2f(u, v);
				glVertex2i(x, y + dy);
				glTexCoord2f(u + du, v);
				glVertex2i(x + dx, y + dy);
				glTexCoord2f(u + du, v + dv);
				glVertex2i(x + dx, y);
			glEnd();
		}

		class TextureRect {
		public:
			TextureRect(void)					{}
			TextureRect(const TextureRect &t)	{ id = t.id;									}
			TextureRect(std::istream &is, Type type = LuminanceMap);
			TextureRect(img::Image &image, Type type = LuminanceMap);

			void bind(void)						{ glBindTexture(GL_TEXTURE_RECTANGLE_NV, id);	}
			void create(void);
			void destroy(void)					{ glDeleteTextures(1, &id);						}

			void copyFromFramebuffer(Format format);

			static void blit(int x, int y, int dx, int dy, 
						float u = 0.0f, float v = 0.0f, float du = 1.0f, float dv = 1.0f);

			static void enable(void)			{ glEnable(GL_TEXTURE_RECTANGLE_NV);			}
			static void disable(void)			{ glDisable(GL_TEXTURE_RECTANGLE_NV);			}
		protected:
			void load(img::Image &image, Type type);

			GLuint id;
		};

		inline void TextureRect::copyFromFramebuffer(Format format)
		{
			glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, format, 0, 0, platform::getScreenWidth(), platform::getScreenHeight(), 0);
		}

		inline void TextureRect::blit(int x, int y, int dx, int dy, 
								float u, float v, float du, float dv)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(u, v + dv);
				glVertex2i(x, y);
				glTexCoord2f(u, v);
				glVertex2i(x, y + dy);
				glTexCoord2f(u + du, v);
				glVertex2i(x + dx, y + dy);
				glTexCoord2f(u + du, v + dv);
				glVertex2i(x + dx, y);
			glEnd();
		}

		class TextureCube {
		public:
			TextureCube(void)					{}
			TextureCube(const TextureCube &t)	{ id = t.id;						}
			TextureCube(img::Image &neg_x, img::Image &pos_x,
					img::Image &neg_y, img::Image &pos_y,
					img::Image &neg_z, img::Image &pos_z, Type type = LuminanceMap);

			void bind(void)				{ glBindTexture(GL_TEXTURE_CUBE_MAP, id);	}

			void destroy(void)			{ glDeleteTextures(1, &id);					}

			static void enable(void)	{ glEnable(GL_TEXTURE_CUBE_MAP);			}
			static void disable(void)	{ glDisable(GL_TEXTURE_CUBE_MAP);			}

			static void enableCubeMapping(void);
			static void disableCubeMapping(void);

		protected:
			GLuint id;
		};

		inline void TextureCube::enableCubeMapping(void)
		{
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		inline void TextureCube::disableCubeMapping(void)
		{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
		}

	}
}

#endif //_include_bleifrei_Texture_hpp_
