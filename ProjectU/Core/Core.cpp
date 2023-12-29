#include "Core.h"
#include "../Gui/Gui.h"
#include <thread>
#include <Windows.h>
#include "Globals.h"
#include "../Protection/xorstr.h"
#include "../Protection/Pop.h"
#include "Input.h"

void Core::Init()
{
	Gui::CreateHWindow(xorstr_("CommonThread"));
	Gui::CreateDevice();
	Gui::CreateImGui();

	SetWindowDisplayAffinity(Gui::window, WDA_EXCLUDEFROMCAPTURE);


	clicker = new Clicker();
	modules.push_back(clicker);

	for (auto module : modules)
	{
		module->Init();
	}
}

void Core::Updates()
{
	do 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		Globals::currentTickCount = GetTickCount64();
		for (auto& module : modules)
		{
			if(!module->isOff)
				module->Update();
		}
	} while (!Globals::exitFlag);
	Globals::exitedThreads++;
}

void Core::Render()
{
	if (Globals::hide)
		SetWindowDisplayAffinity(Gui::window, WDA_EXCLUDEFROMCAPTURE);
	else
		SetWindowDisplayAffinity(Gui::window, WDA_NONE);

	Gui::BeginRender();
	if(showMenu)
		Gui::Render();
	for (auto& module : modules)
	{
		module->Render();
	}
	Gui::EndRender();

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

void Core::RegisterInputs()
{
	g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseEventHandler, GetModuleHandle(NULL), 0);
	g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardEventHandler, GetModuleHandle(NULL), 0);

	MSG msg;

	do
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		/*if (IsDebuggerPresent())
			break;*/
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} while (!Globals::exitFlag);

	UnhookWindowsHookEx(g_hMouseHook);
	UnhookWindowsHookEx(g_hKeyboardHook);

	Globals::exitedThreads++;
}

void Core::Cleanups()
{
	Gui::DestroyImGui();
	Gui::DestroyDevice();
	Gui::DestroyHWindow();


	for (auto module : modules)
	{
		delete module;
	}
}

void Core::ToggleMenu()
{
	if (showMenu)
	{
		showMenu = false;
		ShowWindow(Gui::window, SW_HIDE);
	}
	else
	{
		showMenu = true;
		ShowWindow(Gui::window, SW_SHOW);
		SetWindowDisplayAffinity(Gui::window, WDA_NONE);
		SetWindowDisplayAffinity(Gui::window, WDA_EXCLUDEFROMCAPTURE);
	}
}

LRESULT CALLBACK Core::MouseEventHandler(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			clicker->TurnOn();
			Globals::clickeroff = false;
			break;
		case WM_LBUTTONUP:
			clicker->TurnOff();
			Globals::clickeroff = true;
			break;
		}
	}

	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK Core::KeyboardEventHandler(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (wParam == WM_KEYUP)
		{
			switch (kbdStruct->vkCode)
			{
			case VK_END:
				Globals::exitFlag = true;
				break;
			case VK_INSERT:
				ToggleMenu();
				break;
			}
		}
	}

	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}

void Core::Pop()
{
	PopFile();
}