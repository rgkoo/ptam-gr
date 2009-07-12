
#ifndef __GESTUREGAMEDRIVER_H
#define __GESTUREGAMEDRIVER_H

class GestureGame;
class GestureAnalyzer;

/**
* GestureGameDriver implements a virtual 3d-game by gesture recognition.
*/
class GestureGameDriver{
public:
	GestureGameDriver(GestureGame* game, GestureAnalyzer* gestureAnalyzer):mpGame(game), mpGestureAnalyzer(gestureAnalyzer){}
	void update();	// update game status, invoked by system thread on every frame rendered

protected:
	GestureGame* mpGame;
	GestureAnalyzer* mpGestureAnalyzer;
};

#endif