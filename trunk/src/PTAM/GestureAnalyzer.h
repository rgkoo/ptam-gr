// This header declares the GestureAnalyzer class
// GestureAnalyzer makes and maintains the GestureData struct.
// GestureAnalyzer runs in its own thread, although some functions
// (notably stereo init) are called by the tracker and run in the 
// tracker's thread.

#ifndef __GESTUREANALYZER_H
#define __GESTUREANALYZER_H

#include <cvd/image.h>
#include <cvd/byte.h>
#include <cvd/thread.h>

class GestureAnalyzer:protected CVD::Thread{
public:
	GestureAnalyzer();		// Reset initial parameters and starts thread

	void RequestReset();   // Request that the we reset. Called by the system.
	bool ResetDone();      // Returns true if the has been done.

protected:
	virtual void run();


	

	void reset();



	// Thread interaction signalling stuff
	bool mbResetRequested;   // A reset has been requested
	bool mbResetDone;        // The reset was done.
};


#endif