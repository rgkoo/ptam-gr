
#ifndef _include_Weapon_hpp_
#define _include_Weapon_hpp_ 


// includes
#include <ode/ode.h>

class FPSPlayer;

class Weapon {
public:
	virtual ~Weapon(void);

	virtual void renderFX(void);

	virtual void update(void);
	virtual void firePrimary(void);
	virtual void fireSecondary(void);

protected:
	Weapon(FPSPlayer *player) : player(player) {}

	FPSPlayer *player;
};


#endif //_include_Weapon_hpp_
