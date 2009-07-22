
#ifndef _include_bleifrei_ParticleSystem_hpp_
#define _include_bleifrei_ParticleSystem_hpp_

#pragma warning (disable: 4786)

// includes
#include <list>

#include "../io/ConfigFile.hpp"
#include "../math/Matrix.hpp"
#include "../tex/TextureManager.hpp"

namespace bleifrei {
	namespace fx {

		class ParticleSystem {
		public:
			ParticleSystem(void);
			ParticleSystem(const ParticleSystem &p);
			ParticleSystem(tex::TextureManager &texture_manager, io::ConfigFile &config_file);
			~ParticleSystem(void);

			bool update(void);
			void render(const math::Vector3 &x, const math::Vector3 &y, const math::Vector3 &z);
			void spawn(void);

			void setPosition(math::Vector3 &p)					{ position = p;					}
			void setXDir(math::Vector3 &x)						{ unitx = x;					}
			void setYDir(math::Vector3 &y)						{ unity = y;					}
			void setZDir(math::Vector3 &z)						{ unitz = z;					}

			void enable(void)									{ enabled = true;				}
			void disable(void)									{ enabled = false;				}

			void activate(void)									{ enabled = active = true;		}
			void deactivate(void)								{ active = false;				}

			void die(void)										{ alive = false; deactivate();	}

			void setTexture(tex::Texture2 texture)				{ this->texture = texture;		}

			void setEndSize(float size)							{ endsize = size;				}
			void setStartSize(float size)						{ startsize = size;				}
			void setMinLifetime(float life)						{ minlifetime = life;			}
			void setMaxLifetime(float life)						{ maxlifetime = life;			}
			void setStartAngle(float angle)						{ startangle = angle;			}
			void setEndAngle(float angle)						{ endangle = angle;				}
			void setMinSpawnTime(float time)					{ minspawntime = time;			}
			void setMaxSpawnTime(float time)					{ maxspawntime = time;			}

			void setStartColor(const math::Vector4 &color)		{ startcolor = color;			}
			void setEndColor(const math::Vector4 &color)		{ endcolor = color;				}

			void setDimensions(const math::Vector3 &dim)		{ dimensions = dim;				}
			void setMinVelocity(const math::Vector3 &vel)		{ minvel = vel;					}
			void setMaxVelocity(const math::Vector3 &vel)		{ maxvel = vel;					}
			void setAcceleration(const math::Vector3 &acc)		{ acceleration = acc;			}

			void setMaxParticles(int max)						{ maxparticles = max;			}
			void setSpawnCount(int spawncount)					{ maxspawncount = spawncount;	}

			float getEndSize(void) const						{ return endsize;				}
			float getStartSize(void) const						{ return startsize;				}
			float getMinLifetime(void) const					{ return minlifetime;			}
			float getMaxLifetime(void) const					{ return maxlifetime;			}
			float getStartAngle(void) const						{ return startangle;			}
			float getEndAngle(void) const						{ return endangle;				}
			float getMinSpawnTime(void) const					{ return minspawntime;			}
			float getMaxSpawnTime(void) const					{ return maxspawntime;			}

			const math::Vector4 &getStartColor(void) const		{ return startcolor;			}
			const math::Vector4 &getEndColor(void) const		{ return endcolor;				}

			const math::Vector3 &getDimensions(void) const		{ return dimensions;			}
			const math::Vector3 &getMinVelocity(void) const		{ return minvel;				}
			const math::Vector3 &getMaxVelocity(void) const		{ return maxvel;				}
			const math::Vector3 &getAcceleration(void) const	{ return acceleration;			}

			int getMaxParticles(void)							{ return maxparticles;			}
			int getSpawnCount(void)								{ return maxspawncount;			}

		protected:

			class Particle {
			public:
				Particle(math::Vector3 &pos, math::Vector3 &vel, float lifetime);

				math::Vector3 position;
				math::Vector3 velocity;
				
				float lifetime;
				float age;
			};

			math::Vector3 position;

			math::Vector3 acceleration;
			math::Vector4 startcolor, endcolor;
			math::Vector3 dimensions;
			math::Vector3 minvel, maxvel;
			math::Vector3 unitx, unity, unitz;
			tex::Texture2 texture;

			bool enabled;
			bool active;
			bool alive;

			float spawntime, minspawntime, maxspawntime;
			float startsize, endsize;
			float startangle, endangle;
			float minlifetime, maxlifetime;

			int maxparticles;
			int maxspawncount;

			std::list<Particle *> particles;
		};

		inline ParticleSystem::Particle::Particle(math::Vector3 &pos, math::Vector3 &vel, float lifetime)
			: position(pos), velocity(vel), lifetime(lifetime), age(0)
		{
		}

	}
}

#endif //_include_bleifrei_ParticleSystem_hpp_
