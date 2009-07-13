// This header declares the GestureAnalyzer class
// GestureAnalyzer makes and maintains the GestureData struct.
// GestureAnalyzer runs in its own thread, although some functions
// (notably stereo init) are called by the tracker and run in the 
// tracker's thread.

#ifndef __GESTUREANALYZER_H
#define __GESTUREANALYZER_H

//cvd stuff
#include <cvd/image.h>
#include <cvd/rgb.h>
#include <cvd/byte.h>
#include <cvd/thread.h>

//gesture data definition
#include "GestureData.h"

//std dequeue
#include <deque>


typedef CVD::Image<CVD::Rgb<CVD::byte> > RGBImage;

class GestureAnalyzer:protected CVD::Thread{
public:
	GestureAnalyzer();		// Reset initial parameters and starts thread

	void RequestReset();   // Request that the we reset. Called by the system.
	bool ResetDone();      // Returns true if the has been done.

	void addImage(RGBImage& imageRef);// Invoked by system thread to add camera image.;

	GestureData getGesture();	//get gesture data result, if no gesture result exists, return gesture data whose valid flag is set to false

	std::deque<GestureData>& getGestureDataQueue(){
		return mqGestureDataQueue;
	}

protected:
	virtual void run();	// Load image sequence and recognize gesture

	void analyze();// Invoked by run(), fake method generating;
	

	void reset();



	// Thread interaction signalling stuff
	bool mbResetRequested;   // A reset has been requested
	bool mbResetDone;        // The reset was done.

	std::deque<RGBImage> mqRGBImageQueue;		//Queue of image to be processed
	std::deque<GestureData> mqGestureDataQueue;	//Queue of gesture analysis result
};


#endif