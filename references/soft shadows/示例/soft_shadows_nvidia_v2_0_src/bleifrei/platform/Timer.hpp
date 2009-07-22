/** 
 * bleifrei utility library
 *
 * Copyright (C) 2003 Ulf Borgenstam, Jonas Svensson.
 *
 * Do not distribute. If you know someone interested in obtaining these sources
 * please have them contact us.
 */


#ifndef _include_bleifrei_Timer_hpp_
#define _include_bleifrei_Timer_hpp_


// includes
#include "types.hpp"

namespace bleifrei {
	namespace platform {

		class Timer {
		public:
			Timer(dword time = 0)		{ reset(time);									}

			void reset(dword time = 0)	{ lastTime = GetTickCount(); baseTime = time;	}
			dword getTime(void)			{ return GetTickCount() - lastTime + baseTime;	}
			dword getTimeAndReset()		{
				dword currentTime = GetTickCount();
				dword frameTime = (currentTime - lastTime);
				lastTime = currentTime;
				return frameTime;
			}

		protected:
			dword baseTime;
			dword lastTime;
		};

	}
}

#endif //_include_bleifrei_Timer_hpp_
