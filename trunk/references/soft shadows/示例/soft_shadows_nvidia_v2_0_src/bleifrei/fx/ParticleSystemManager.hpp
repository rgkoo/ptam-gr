
#ifndef _include_bleifrei_ParticleSystemManager_hpp_
#define _include_bleifrei_ParticleSystemManager_hpp_

// includes
#include <list>

#include "ParticleSystem.hpp"

namespace bleifrei {
	namespace fx {

		class ParticleSystemManager {
		public:
			ParticleSystemManager(void);
			~ParticleSystemManager(void);

			void add(ParticleSystem *p);

			void update(void);
			void render(void);

		protected:
			std::list<ParticleSystem *> particle_systems;
		};

	}
}

#endif //_include_bleifrei_ParticleSystemManager_hpp_