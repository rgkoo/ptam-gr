/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_Material_hpp_
#define _include_bleifrei_Material_hpp_


// includes
#include <iostream>

#include "../gfx/opengl.hpp"
#include "Color.hpp"

namespace bleifrei {
	namespace gfx {

		class Material {
		public:
			Material(std::istream &is);
			Material(	Color &amb	= Color(1.0f, 1.0f, 1.0f),
						Color &diff = Color(1.0f, 1.0f, 1.0f),
						Color &spec = Color(),
						Color &emi	= Color(), float s = 25.0f,	bool t = false);
			void bind(void);
			void write(std::ostream &os);

			bool transparent;
			Color ambient;
			Color diffuse;
			Color specular;
			Color emission;
			float shininess;
		};

		inline Material::Material(Color &amb, Color &diff, Color &spec, Color &emi, float s, bool t) {
			ambient = amb;
			diffuse = diff;
			specular = spec;
			emission = emi;
			shininess = s;
			transparent = t;
		}

		inline void Material::bind(void) {
			// TODO: transparency
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular[0]);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emission[0]);
		}

	}
}

#endif //_include_bleifrei_Material_hpp_
