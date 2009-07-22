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
#include <string>

#include "../io/console.hpp"
#include "VertexProgram.hpp"

using namespace bleifrei::io;
using namespace std;

namespace bleifrei {
	namespace gfx {

		VertexProgram::VertexProgram(istream &is)
		{
			if (!GL_ARB_vertex_program_initialized) {
				console::print(1, 0, 0, "Error(VertexProgram): GL_ARB_vertex_program not initialized");
				return;
			}

			string program;

			char ch;
			while (is.get(ch)) program += ch;

			glGetError();
			glGenProgramsARB(1, &id);
			if (glGetError()) {
				console::print(1.0, 0.0, 0.0, "Could not generate VertexProgram id!");
				return;
			}
			glBindProgramARB(GL_VERTEX_PROGRAM_ARB, id);
			glGetError();
			glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)program.size(), program.c_str());
			if (glGetError()) {
				console::print(1.0f, 0.0f, 0.0f, (char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB));
			}
			// TODO: throw exception on error
		}

	}
}