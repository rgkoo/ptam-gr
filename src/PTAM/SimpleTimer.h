
#ifndef __SIMPLETIMER_H
#define __SIMPLETIMER_H

//This is Windows timer implementation.

struct TimerStruct
{
	__int64       frequency;						// 频率
	float         resolution;						// 时间间隔
	unsigned long mm_timer_start;					// 多媒体计时器的开始时间
	unsigned long mm_timer_elapsed;				// 多媒体计时器的开始时间
	bool		performance_timer;					// 使用Performance Timer?
	__int64       performance_timer_start;		// Performance Timer计时器的开始时间
	__int64       performance_timer_elapsed;		// Performance Timer计时器的开始时间
} ;

//TODO: modify this to enable multiple timers
class SimpleTimer{
public:
	void initTimer();									// 初始化我们的计时器
	float getTime();									// 返回经过的时间，以毫秒为单位

protected:
	TimerStruct mTimerStruct;
};

#endif