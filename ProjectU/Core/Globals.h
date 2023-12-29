#pragma once
#include <Windows.h>
#include <atomic>

namespace Globals
{
	inline ULONGLONG currentTickCount = 0;

	inline bool clickeroff = true;
	inline bool hide = true;

	inline const char* DEBUG = "";

	inline bool exitFlag = false;

	inline bool clicker = false;
	inline float cps = 12.3f;
	inline float lowerCps = 10.f;
	inline float upperCps = 13.2f;

	inline bool cpsRange = false;

	inline bool jitter = false;

	inline HWND targetWindow = NULL;

	inline std::atomic<int> exitedThreads = 0;



	inline int demoInt = 0;
}