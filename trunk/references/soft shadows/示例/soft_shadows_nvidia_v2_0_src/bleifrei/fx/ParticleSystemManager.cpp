
// includes
#include "../math/Matrix_op.hpp"
#include "../gfx/opengl.hpp"

#include "ParticleSystem.hpp"
#include "ParticleSystemManager.hpp"

using namespace bleifrei::io;
using namespace bleifrei::math;
using namespace std;

namespace bleifrei {
	namespace fx {

		ParticleSystemManager::ParticleSystemManager(void)
		{
		}

		ParticleSystemManager::~ParticleSystemManager(void)
		{
			while (!particle_systems.empty()) {
				delete particle_systems.back();
				particle_systems.pop_back();
			}
		}

		void ParticleSystemManager::add(ParticleSystem *p)
		{
			particle_systems.push_back(p);
		}

		void ParticleSystemManager::update(void)
		{
			list<ParticleSystem *>::iterator it = particle_systems.begin();

			while (it != particle_systems.end()) {
				if (!(*it)->update()) {
					delete (*it);
					it = particle_systems.erase(it);
				}
				else {
					++it;
				}
			}
		}

		void ParticleSystemManager::render(void)
		{
			Matrix4x4 modelview_matrix_transpose;

			glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, &modelview_matrix_transpose[0]);

			const Vector3 &x = *(Vector3 *)&modelview_matrix_transpose.getColumn(0);
			const Vector3 &y = *(Vector3 *)&modelview_matrix_transpose.getColumn(1);
			const Vector3 &z = *(Vector3 *)&modelview_matrix_transpose.getColumn(2);

			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			list<ParticleSystem *>::iterator it;

			glColor4f(1,1,1,1);
			for (it = particle_systems.begin(); it != particle_systems.end(); it++) {
				(*it)->render(x, y, z);
			}

			glDisable(GL_BLEND);
			glDepthMask(GL_TRUE);
		}

	}
}