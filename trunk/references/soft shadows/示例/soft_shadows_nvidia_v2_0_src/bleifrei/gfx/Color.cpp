/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "Color.hpp"
#include "../io/io.hpp"

using namespace bleifrei::io;
using namespace std;

namespace bleifrei {
	namespace gfx {

		Color::Color(istream &is) {
			get(is, r());
			get(is, g());
			get(is, b());
			get(is, a());
		}

		void Color::write(ostream &os) {
			put(os, r());
			put(os, g());
			put(os, b());
			put(os, a());
		}

	}
}
