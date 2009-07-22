/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Color_hpp_
#define _include_bleifrei_Color_hpp_


// includes
#include <iostream>

namespace bleifrei {
	namespace gfx {

		class Color {
		public:
			Color(std::istream &is);
			Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
			Color(const Color &c);

			float &operator [](int index)	{ return elem[index];	}
			Color operator *(float factor);
			Color operator +(Color &c);
			Color operator -(Color &c);

			float &r(void)					{ return elem[0];		}
			float &g(void)					{ return elem[1];		}
			float &b(void)					{ return elem[2];		}
			float &a(void)					{ return elem[3];		}
			
			void write(std::ostream &os);

		protected:
			float elem[4];
		};

		inline Color::Color(float r, float g, float b, float a) {
			this->r() = r;
			this->g() = g;
			this->b() = b;
			this->a() = a;
		}

		inline Color::Color(const Color &c) {
			elem[0] = c.elem[0];
			elem[1] = c.elem[1];
			elem[2] = c.elem[2];
			elem[3] = c.elem[3];
		}

		inline Color Color::operator *(float factor) {
			return Color(factor * r(), factor * g(), factor * b(), factor * a());
		}

		inline Color Color::operator +(Color &c) {
			return Color(r() + c.r(), g() + c.g(), b() + c.b(), a() + c.a());
		}

		inline Color Color::operator -(Color &c) {
			return Color(r() - c.r(), g() - c.g(), b() - c.b(), a() - c.a());
		}

		inline Color operator *(float factor, Color &c) {
			return c * factor;
		}

	}
}

#endif //_include_bleifrei_Color_hpp_
