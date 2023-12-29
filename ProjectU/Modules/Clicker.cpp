#pragma once
#include "Module.h"
#include <Windows.h>
#include <iostream>
#include <random>
#include <thread>
#include <atomic>

#include "../Core/Input.h"
#include "../Core/Globals.h"
#include "../Gui/Gui.h"
#include <string>

class Clicker : public Module
{
private:
	std::atomic<bool> terminateThread = false;

	float delay = 0;
	float rDelay = 0;
	ULONGLONG lastClick = 0;
	ULONGLONG lastTimeInactive = 0;
	ULONGLONG timeActive = 0;
	HWND currentWindow = NULL;

	POINT mousePosition{ 0, 0 };
	POINT clickPosition{ 0, 0 };


	int random(int min, int max) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> x(min, max);

		return (int)x(rng);
	}

	float random(float min, float max) {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> x(min, max);

		return x(rng);
	}

	void UpdateCps()
	{
		while (!terminateThread){
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (Globals::cpsRange)
			{
				int randomCps = random(Globals::lowerCps, Globals::upperCps);
				delay = 1000.f / randomCps;
			}
			else
				delay = 1000.f / Globals::cps;
			Globals::DEBUG = std::to_string(delay).c_str();
		}
	}

public:
	Clicker()
	{
		std::thread updateValueThread(&Clicker::UpdateCps, this);
		updateValueThread.detach();
	}
	~Clicker()
	{
		terminateThread = true;
	}

	void Init() override
	{
		delay = 1000.f / Globals::cps;
	}
	void Update() override
	{
		if (Globals::clicker && Globals::currentTickCount - lastClick >= delay)
		{
			//timeActive = Globals::currentTickCount - lastTimeInactive;
			currentWindow = GetForegroundWindow();

			if ((Globals::targetWindow && currentWindow != Globals::targetWindow) || currentWindow == Gui::window)
				return;
			if (!GetCursorPos(&mousePosition) || !ScreenToClient(currentWindow, &mousePosition))
				return;

			//PostMessage(currentWindow, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(mousePosition.x, mousePosition.y));
			//PostMessage(currentWindow, WM_LBUTTONUP, 0, MAKELPARAM(mousePosition.x, mousePosition.y));
			//lastClick = GetTickCount64();

			lastClick = Input::SendLeftClick(mousePosition, currentWindow);
		}
		else
		{
			//lastTimeInactive = Globals::currentTickCount;
			//timeActive = 0;
		}
	}
	void Render() override
	{

	}
};