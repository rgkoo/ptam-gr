#include "GestureGameDriver.h"

#include "TeapotGame.h"
#include "GestureAnalyzer.h"

//extern variables
#include <iostream>
using namespace std;


void GestureGameDriver::update()
{
	//fetch gesture result from analyzer
	GestureData& gesture = mpGestureAnalyzer->getGesture();
	//valid gesture data
	if(gesture.mbIsValid){
		cout<<"gesture ok"<<endl;

		//drive gesture game
		mpGame->onGesture(gesture);
	}

}
