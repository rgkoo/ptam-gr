#ifndef _include_TractorBeam_hpp_
#define _include_TractorBeam_hpp_ 


// includes
#include <ode/ode.h>

#include "bleifrei/math/Matrix.hpp"
#include "bleifrei/tex/Texture.hpp"

#include "Weapon.hpp"

class TractorBeam : public Weapon {
public:
	TractorBeam(FPSPlayer *player, bleifrei::tex::Texture2 texture);
	~TractorBeam(void);

	void update(void);
	void firePrimary(void);
	void fireSecondary(void);

protected:
	bool stop_primary, stop_secondary;
	bool no_throw;

	dBodyID primary_target, secondary_target;
	bleifrei::math::Vector3 target_position, position;
	float distance;

	bleifrei::tex::Texture2 texture;
};


#endif //_include_TractorBeam_hpp_
