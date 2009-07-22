/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

#ifndef _include_bleifrei_VertexProgram_hpp_
#define _include_bleifrei_VertexProgram_hpp_ 


// includes
#include <iostream>

#include "../math/Matrix.hpp"
#include "../gfx/opengl.hpp"

namespace bleifrei {
	namespace gfx {

		class VertexProgram {
		public:
			VertexProgram(void)							{}
			VertexProgram(const VertexProgram &vp)		{ id = vp.id;									}
			VertexProgram(std::istream &is);

			void bind(void)								{ glBindProgramARB(GL_VERTEX_PROGRAM_ARB, id);	}
			void destroy(void)							{ glDeleteProgramsARB(1, &id);					}

			static void localParameter(GLuint index, float x, float y = 0.0f, float z = 0.0f, float w = 0.0f);
			static void localParameter(GLuint index, const math::Matrix<4, 4, float> &mat);
			static void localParameter(GLuint index, const math::Matrix<4, 1, float> &vec); 

			static void enable(void)					{ glEnable(GL_VERTEX_PROGRAM_ARB);				}
			static void disable(void)					{ glDisable(GL_VERTEX_PROGRAM_ARB);				}
		private:
			GLuint id;
		};

		inline void VertexProgram::localParameter(GLuint index, float x, float y, float z, float w)
		{
			glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, index, x, y, z, w);
		}

		inline void VertexProgram::localParameter(GLuint index, const math::Matrix<4, 4, float> &mat)
		{
			glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, index,		mat(0,0), mat(0,1), mat(0,2), mat(0,3));
			glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, index + 1,	mat(1,0), mat(1,1), mat(1,2), mat(1,3));
			glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, index + 2,	mat(2,0), mat(2,1), mat(2,2), mat(2,3));
			glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, index + 3,	mat(3,0), mat(3,1), mat(3,2), mat(3,3));
		}

		inline void VertexProgram::localParameter(GLuint index, const math::Matrix<4, 1, float> &vec)
		{
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB, index,	&vec[0]);
		}

	}
}

#endif //_include_bleifrei_VertexProgram_hpp_ 
