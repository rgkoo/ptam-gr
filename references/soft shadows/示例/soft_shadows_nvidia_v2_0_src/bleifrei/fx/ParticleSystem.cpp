
// includes
#include "../gfx/opengl.hpp"
#include "../math/Matrix_op.hpp"
#include "../math/misc.hpp"
#include "../tex/TextureManager.hpp"

#include "ParticleSystem.hpp"

using namespace bleifrei::io;
using namespace bleifrei::math;
using namespace bleifrei::tex;
using namespace std;

namespace bleifrei {
	namespace fx {

		ParticleSystem::ParticleSystem(void)
			: position(0, 0, 0), unitx(1,0,0), unity(0,1,0), unitz(0,0,1), enabled(true), active(true), alive(true)
		{
		}

		ParticleSystem::ParticleSystem(const ParticleSystem &particle_system)
		{
			dimensions = particle_system.dimensions;

			maxparticles = particle_system.maxparticles;
			minspawntime = particle_system.minspawntime;
			maxspawntime = particle_system.maxspawntime;
			maxspawncount = particle_system.maxspawncount;

			minvel = particle_system.minvel;
			maxvel = particle_system.maxvel;
			acceleration = particle_system.acceleration;

			startsize = particle_system.startsize;
			endsize = particle_system.endsize;
			startangle = particle_system.startangle;
			endangle = particle_system.endangle;
			minlifetime = particle_system.minlifetime;
			maxlifetime = particle_system.maxlifetime;
			texture = particle_system.texture;

			startcolor = particle_system.startcolor;
			endcolor = particle_system.endcolor;

			unitx = particle_system.unitx;
			unity = particle_system.unity;
			unitz = particle_system.unitz;

			position = particle_system.position;

			enabled = particle_system.enabled;
			active = particle_system.active;
			alive = particle_system.alive;

			spawntime = 0.0;
		}

		ParticleSystem::ParticleSystem(TextureManager &texture_manager, ConfigFile &config_file)
			: position(0, 0, 0), unitx(1,0,0), unity(0,1,0), unitz(0,0,1), enabled(true), active(true), alive(true)
		{
			string texture;

			config_file.selectSection("ParticleSystem");
			config_file.get("dimensions", dimensions);
			config_file.get("maxparticles", maxparticles);
			config_file.get("minspawntime", minspawntime);
			config_file.get("maxspawntime", maxspawntime);
			config_file.get("maxspawncount", maxspawncount);

			config_file.selectSection("SpawnVelocity");
			config_file.get("min", minvel);
			config_file.get("max", maxvel);
			config_file.get("acceleration", acceleration);

			config_file.selectSection("Particle");
			config_file.get("texture", texture);
			config_file.get("startsize", startsize);
			config_file.get("endsize", endsize);
			config_file.get("minlifetime", minlifetime);
			config_file.get("maxlifetime", maxlifetime);
			config_file.get("startangle", startangle);
			config_file.get("endangle", endangle);
			config_file.get("startcolor", startcolor);
			config_file.get("endcolor", endcolor);

			this->texture = texture_manager.load(texture);
			spawntime = 0.0;
		}

		ParticleSystem::~ParticleSystem(void)
		{
			while (!particles.empty()) {
				delete particles.back();
				particles.pop_back();
			}
		}

		bool ParticleSystem::update(void)
		{
			float step = platform::getFrameStep();

			if (enabled) {
				list<Particle *>::iterator it = particles.begin();
				if (it == particles.end()) {
					if (!alive) return false;
					if (!active) disable();
				}
				while (it != particles.end()) {
					if (((*it)->age += step) > (*it)->lifetime) {
						delete (*it);
						it = particles.erase(it);
						continue;
					}

					(*it)->position += step * (*it)->velocity;
					(*it)->velocity += step * acceleration;
					it++;
				}

				if (!active) return true;

				if ((spawntime -= step) < 0.0) {
					spawntime = random_float(minspawntime, maxspawntime);

					int spawncount = maxparticles - (int)particles.size();

					if (spawncount > maxspawncount) spawncount = maxspawncount;

					for (int i = 0; i < spawncount; i++) {
						spawn();
					}
				}
			}
			return true;
		}

		void ParticleSystem::render(const Vector3 &x, const Vector3 &y, const Vector3 &z)
		{
			if (enabled) {
				texture.bind();

				list<Particle *>::iterator it;
				for (it = particles.begin(); it != particles.end(); it++) {
					float factor = (*it)->age / (*it)->lifetime;
					float size = startsize + factor * (endsize - startsize);
					float angle = startangle + factor * (endangle - startangle);

					glPushMatrix();
						glTranslatef((*it)->position[0], (*it)->position[1], (*it)->position[2]);
						glRotatef(angle, z[0], z[1], z[2]);
						glColor4fv((float *)&(startcolor + factor * (endcolor - startcolor)));
/*						glBegin(GL_TRIANGLE_FAN);
							glTexCoord2f(0, 0); glVertex3fv((float *)&((*it)->position - (x + y) * size));
							glTexCoord2f(1, 0); glVertex3fv((float *)&((*it)->position + (x - y) * size));
							glTexCoord2f(1, 1); glVertex3fv((float *)&((*it)->position + (x + y) * size));
							glTexCoord2f(0, 1); glVertex3fv((float *)&((*it)->position - (x - y) * size));
						glEnd();
*/
						glBegin(GL_TRIANGLE_FAN);
							glTexCoord2f(0, 0); glVertex3fv((float *)&(-(x + y) * size));
							glTexCoord2f(1, 0); glVertex3fv((float *)&( (x - y) * size));
							glTexCoord2f(1, 1); glVertex3fv((float *)&( (x + y) * size));
							glTexCoord2f(0, 1); glVertex3fv((float *)&(-(x - y) * size));
						glEnd();
					glPopMatrix();
				}
			}
		}

		void ParticleSystem::spawn(void)
		{
			Vector3 pos(0.5f * random_float(-dimensions[0], dimensions[0]) + position[0],
						0.5f * random_float(-dimensions[1], dimensions[1]) + position[1],
						0.5f * random_float(-dimensions[2], dimensions[2]) + position[2]);
			Vector3 vel =  unitx * random_float(minvel[0], maxvel[0]) +
							unity * random_float(minvel[1], maxvel[1]) + 
							unitz * random_float(minvel[2], maxvel[2]);

			particles.push_back(new Particle(pos, vel, random_float(minlifetime, maxlifetime)));
		}
	}
}
