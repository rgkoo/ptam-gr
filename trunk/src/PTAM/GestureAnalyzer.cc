
#include <TooN/helpers.h>

#include "GestureAnalyzer.h"

//#define __DEBUG

#ifdef __DEBUG
#include "gvars3/instances.h"
using namespace CVD;
using namespace std;
using namespace GVars3;
#endif


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


void GestureAnalyzer::reset()
{
	mbResetDone = true;
	mbResetRequested = false;
}
// CHECK_RESET is a handy macro which makes the mapmaker thread stop
// what it's doing and reset, if required.
#define CHECK_RESET if(mbResetRequested) {reset(); continue;};


void GestureAnalyzer::run()
{
#ifdef WIN32
	// For some reason, I get tracker thread starvation on Win32 when
	// adding key-frames. Perhaps this will help:
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
#endif
	while(!shouldStop())  // ShouldStop is a CVD::Thread func which return true if the thread is told to exit.
	{
		CHECK_RESET;
		sleep(5); // Sleep not really necessary, especially if mapmaker is busy
		CHECK_RESET;

		#ifdef __DEBUG
		//just for test
		//GUI.ParseLine("test");
		std::cout<<"test"<<std::endl;
		#endif

		analyze();
		CHECK_RESET;
	}
}

GestureAnalyzer::GestureAnalyzer()
{
	reset();
	start();
}

void GestureAnalyzer::analyze() /* Invoked by run(), fake method generating */
{
	RGBImage* image;
	//TODO:do synchronize here
	if(mqRGBImageQueue.size() > 0){
		image = mqRGBImageQueue.front();
		//analyze the image
		mqRGBImageQueue.pop();

		GestureData* gestureData = new GestureData;

		//TODO:
		gestureData->mCenterPosition = makeVector(4,4,0);

		//TODO:do synchronize here
		mqGestureDataQueue.push(gestureData);
	}
}
