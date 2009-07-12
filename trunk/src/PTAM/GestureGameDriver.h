
#ifndef __GESTUREGAMEDRIVER_H
#define __GESTUREGAMEDRIVER_H

class TeapotGame;
class GestureAnalyzer;

/**
* GestureGameDriver implements a virtual 3d-game by gesture recognition.
*/
class GestureGameDriver{
public:
	GestureGameDriver(TeapotGame* game, GestureAnalyzer* gestureAnalyzer):mpGame(game), mpGestureAnalyzer(gestureAnalyzer){}
	void update();	// update game status, invoked by system thread on every frame rendered

protected:
	TeapotGame* mpGame;
	GestureAnalyzer* mpGestureAnalyzer;
};

#endif