/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */

// includes
#include "../tex/TextureManager.hpp"
#include "../platform/types.hpp"

#include "Skybox.hpp"

using namespace bleifrei::io;
using namespace bleifrei::mesh;
using namespace bleifrei::tex;
using namespace bleifrei;
using namespace std;

namespace bleifrei {
	namespace gfx {

		Skybox::Skybox(void) {
		}

		Skybox::~Skybox(void) {
		}

		void Skybox::load(TextureManager &tm, const string file0, const string file1, const string file2, const string file3, const string file4, int width, int height, int length) {
			this->texture[0] = tm.load(file0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP);
			this->texture[1] = tm.load(file1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP);
			this->texture[2] = tm.load(file2);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP);
			this->texture[3] = tm.load(file3);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP);
			this->texture[4] = tm.load(file4);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP);

			platform::dword id = glGenLists(1);

			glNewList(id, GL_COMPILE);

				// top
				this->texture[0].bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,0); glVertex3i(-width/2, height, -length/2);
					glTexCoord2f(0,1); glVertex3i( width/2, height, -length/2);
					glTexCoord2f(1,1); glVertex3i( width/2, height,  length/2);
					glTexCoord2f(1,0); glVertex3i(-width/2, height,  length/2);
				glEnd();

				// left
				this->texture[1].bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,1); glVertex3i(-width/2, height, -length/2);
					glTexCoord2f(1,1); glVertex3i(-width/2, height,  length/2);
					glTexCoord2f(1,0); glVertex3i(-width/2,      0,  length/2);
					glTexCoord2f(0,0); glVertex3i(-width/2,      0, -length/2);
				glEnd();

				// front
				this->texture[2].bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,1); glVertex3i(-width/2, height,  length/2);
					glTexCoord2f(1,1); glVertex3i( width/2, height,  length/2);
					glTexCoord2f(1,0); glVertex3i( width/2,		 0,  length/2);
					glTexCoord2f(0,0); glVertex3i(-width/2,		 0,  length/2);
				glEnd();

				// right
				this->texture[3].bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,1); glVertex3i( width/2, height,  length/2);
					glTexCoord2f(1,1); glVertex3i( width/2, height, -length/2);
					glTexCoord2f(1,0); glVertex3i( width/2,      0, -length/2);
					glTexCoord2f(0,0); glVertex3i( width/2,		 0,  length/2);
				glEnd();

				// back
				this->texture[4].bind();
				glBegin(GL_QUADS);
					glTexCoord2f(0,1); glVertex3i( width/2, height, -length/2);
					glTexCoord2f(1,1); glVertex3i(-width/2, height, -length/2);
					glTexCoord2f(1,0); glVertex3i(-width/2,		 0, -length/2);
					glTexCoord2f(0,0); glVertex3i( width/2,		 0, -length/2);
				glEnd();

			glEndList();
			
			model = Model(id);
		}

		void Skybox::render(void) {
			glColor4f(1, 1, 1, 1);
			model.render();
		}

	}
}