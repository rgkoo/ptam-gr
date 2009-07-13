

#include "SimpleTimer.h"
#include <Windows.h>
#include  <MMSystem.h>



void SimpleTimer::initTimer()									// 初始化我们的计时器
{
	memset(&mTimerStruct, 0, sizeof(mTimerStruct));					// 清空计时器结构

	// 检测Performance Counter是否可用，可用则创建
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &mTimerStruct.frequency))
	{
		// 如果不可用
		mTimerStruct.performance_timer	= FALSE;				// 设置Performance Timer为false
		mTimerStruct.mm_timer_start	= timeGetTime();		// 使用普通的计时器
		mTimerStruct.resolution	= 1.0f/1000.0f;				// 设置单位为毫秒
		mTimerStruct.frequency		= 1000;						// 设置频率为1000
		mTimerStruct.mm_timer_elapsed	= mTimerStruct.mm_timer_start;	// 设置流失的时间为当前的时间
	}
	else
	{
		// 使用Performance Counter计时器
		QueryPerformanceCounter((LARGE_INTEGER *) &mTimerStruct.performance_timer_start);
		mTimerStruct.performance_timer		= TRUE;				// 设置Performance Timer为TRUE
		// 计算计时的精确度
		mTimerStruct.resolution		= (float) (((double)1.0f)/((double)mTimerStruct.frequency));
		// 设置流失的时间为当前的时间
		mTimerStruct.performance_timer_elapsed	= mTimerStruct.performance_timer_start;
	}
}

float SimpleTimer::getTime()									// 返回经过的时间，以毫秒为单位
{
	__int64 time;										// 使用64位的整数

	if (mTimerStruct.performance_timer)						// 是否使用Performance Timer计时器?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);		// 返回当前的时间
		// 返回时间差
		return ( (float) ( time - mTimerStruct.performance_timer_start) * mTimerStruct.resolution)*1000.0f;
	}
	else
	{
		// 使用普通的计时器，返回时间差
		return( (float) ( timeGetTime() - mTimerStruct.mm_timer_start) * mTimerStruct.resolution)*1000.0f;
	}
}