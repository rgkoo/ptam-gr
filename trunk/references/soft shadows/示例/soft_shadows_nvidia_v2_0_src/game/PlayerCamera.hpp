
#ifndef _include_PlayerCamera_hpp_
#define _include_PlayerCamera_hpp_ 


// includes
#include "Camera.hpp"
#include "FPSPlayer.hpp"

class PlayerCamera : public Camera {
public:
	PlayerCamera(FPSPlayer &player)
		: player(player)
	{
	}

	void update(void);

protected:
	FPSPlayer &player;
};


#endif //_include_PlayerCamera_hpp_
