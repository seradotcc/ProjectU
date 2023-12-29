#pragma once
#include <Windows.h>

namespace Input
{
	inline ULONGLONG SendLeftClick(POINT mousePosition, HWND window = GetForegroundWindow())
	{
		PostMessage(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(mousePosition.x, mousePosition.y));
		PostMessage(window, WM_LBUTTONUP, 0, MAKELPARAM(mousePosition.x, mousePosition.y));
		return GetTickCount64();
	}

	inline ULONGLONG SendLeftClick(HWND window = GetForegroundWindow())
	{
		PostMessage(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
		PostMessage(window, WM_LBUTTONUP, 0, MAKELPARAM(0, 0));
		return GetTickCount64();
	}
}