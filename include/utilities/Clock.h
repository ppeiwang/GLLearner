#pragma once

#include "RenderCoreDefine.h"

RENDER_CORE_BEGIN

class Clock
{
public:

	/**
	*  @private
	*  @brief Move to next frame, update time
	*/
	static void NextFrame();

	/**
	*  @private
	*  @brief ResetTimer
	*/
	static void Reset();

	/**
	*  @brief Get current frame sequence number
	*/
	static unsigned long long GetFrame() { return mFrame; }

	/**
	*  @brief Get current time millisecond
	*/
	static unsigned long long GetMs() { return mMs; }

	/**
	*  @brief Get current time nanosecond remainder
	*/
	static unsigned long long GetModNs() { return mModNs; }

	/**
	*  @brief Get delta millisecond from last tick
	*/
	static unsigned long long GetDeltaMs() { return mDeltaMs; }

	/**
	*  @brief Get delta nanosecond remainder from last tick
	*/
	static unsigned long long GetDeltaModNs() { return mDeltaModNs; }

	/**
	*  @brief Get current system time millisecond count from 1970.1.1
	*/
	static long long GetSysTimeMs();
private:
	static unsigned long long mFrame;
	static unsigned long long mMs;
	static unsigned long long mModNs;
	static unsigned long long mDeltaMs;
	static unsigned long long mDeltaModNs;
	static unsigned long long mSysOneSecondTickCount;
	static unsigned long long mSysTickNs;
};

RENDER_CORE_END