#include "GestureGameDriver.h"

#include "TeapotGame.h"
#include "GestureAnalyzer.h"

//extern variables
#include <iostream>
using namespace std;


void GestureGameDriver::update()
{
	//fetch gesture result from analyzer
	GestureData gestureData = mpGestureAnalyzer->getGesture();
	//valid gesture data
	if(gestureData.mbIsValid){
		cout<<"gesture ok:"<<gestureData.left<<" "<<gestureData.top<<" "<<gestureData.width<<" "<<gestureData.height<<endl;

		//drive gesture game
		mpGame->onGesture(gestureData);
	}

}
