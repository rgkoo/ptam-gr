
#include <TooN/helpers.h>

#include "GestureAnalyzer.h"

#define __DEBUG

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


pthread_mutex_t imageQueueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t resultQueueMutex = PTHREAD_MUTEX_INITIALIZER;



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
		//std::cout<<"test"<<std::endl;
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
	RGBImage image;
	GestureData gestureData;
	
	if(mqRGBImageQueue.size() > 0){

		//do synchronize here
		pthread_mutex_lock(&imageQueueMutex);

	#ifdef __DEBUG
			cout<<"image queue size:"<<mqRGBImageQueue.size()<<endl;
	#endif
			//get image to analyze
			image = mqRGBImageQueue.front();
			//pop out from the queue
			mqRGBImageQueue.pop_front();

			//in case that image processing is too slow
			if(mqRGBImageQueue.size() > 10){
				mqRGBImageQueue.clear();
			}

		//release image queue lock
		pthread_mutex_unlock(&imageQueueMutex);


		//TODO: do real gesture recognition
		for(int i=0;i<50000000;i++){
			;	
		}
		gestureData.mCenterPosition = makeVector(4,4,0);

		//do synchronize here
		pthread_mutex_lock(&resultQueueMutex);
			mqGestureDataQueue.push_back(gestureData);
		pthread_mutex_unlock(&resultQueueMutex);
	}
}

void GestureAnalyzer::addImage( RGBImage& imageRef ) /* Invoked by system thread to add camera image. */
{
	//do synchronize
	pthread_mutex_lock(&imageQueueMutex);

		//RGBImage* image = new RGBImage;
		//*image = imageRef;
		mqRGBImageQueue.push_back(imageRef);

	//release lock
	pthread_mutex_unlock(&imageQueueMutex);
}

GestureData GestureAnalyzer::getGesture()
{
	GestureData gesture;
	if(mqGestureDataQueue.size() > 0){
	
	//do synchronize here
	pthread_mutex_lock(&resultQueueMutex);
		//get gesture data
		gesture = mqGestureDataQueue.front();
		//pop from the queue
		mqGestureDataQueue.pop_front();
	pthread_mutex_unlock(&resultQueueMutex);

	//set as valid gesture
	gesture.mbIsValid = true;

	}
	else{
		gesture.mbIsValid = false;
	}

	return gesture;
}