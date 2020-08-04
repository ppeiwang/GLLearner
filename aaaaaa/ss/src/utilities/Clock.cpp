#include "Clock.h"
#include <chrono>
#include <windows.h>
#include <assert.h>

namespace
{
	unsigned long long GetHPFrequency()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}

	unsigned long long GetHPCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
	}
}

typedef unsigned long long ClockStamp;
unsigned long long Clock::mFrame = 0;
unsigned long long Clock::mMs = 0;
unsigned long long Clock::mModNs = 0;
unsigned long long Clock::mDeltaMs = 0;
unsigned long long Clock::mDeltaModNs = 0;
unsigned long long Clock::mSysOneSecondTickCount = GetHPFrequency();
unsigned long long Clock::mSysTickNs = (unsigned long long)(1000000000 / mSysOneSecondTickCount);

void Clock::NextFrame()
{
	static unsigned long long sLastTimeStamp = 0;
	if (sLastTimeStamp == 0)
	{
		assert(mSysOneSecondTickCount != 0, "Fetch system timer tick count failed");
		assert(mSysTickNs != 0, "Calculate time frequency ns failed");
		sLastTimeStamp = GetHPCounter();
	}
	else
	{
		unsigned long long sTimeStamp = GetHPCounter();
		unsigned long long sTimeDeltaStamp = sTimeStamp - sLastTimeStamp;
		sLastTimeStamp = sTimeStamp;
		unsigned long long sDeltaNs = sTimeDeltaStamp * mSysTickNs;
		mDeltaMs = sDeltaNs / 1000000;
		mDeltaModNs = sDeltaNs % 1000000;
		mModNs = mModNs + mDeltaModNs;
		mMs = mMs + mDeltaMs + mModNs / 1000000;
		mModNs = mModNs % 1000000;
	}
	mFrame++;
}

void Clock::Reset()
{
	mFrame = 0;
	mMs = 0;
	mModNs = 0;
}

long long Clock::GetSysTimeMs()
{
	std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(t);
	auto epoch = now_ms.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	return value.count();
}
