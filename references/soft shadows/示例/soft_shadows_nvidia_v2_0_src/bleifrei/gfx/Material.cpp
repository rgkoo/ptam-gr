/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "Material.hpp"
#include "../io/io.hpp"

using namespace bleifrei::io;
using namespace std;

namespace bleifrei {
	namespace gfx {

		Material::Material(istream &is) {
			ambient = Color(is);
			diffuse = Color(is);
			specular = Color(is);
			emission = Color(is);

			get(is, shininess);
			get(is, transparent);
		}

		void Material::write(ostream &os) {
			ambient.write(os);
			diffuse.write(os);
			specular.write(os);
			emission.write(os);
			put(os, shininess);
			put(os, transparent);
		}


	}
}