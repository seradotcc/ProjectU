#pragma once
#include <vector>

#include "../Modules/Module.h"
#include "../Modules/Clicker.cpp"

namespace Core
{
	static Module* clicker = nullptr;

	static std::vector<Module*> modules;
	static bool showMenu = true;

	void Init();
	void Updates();
	void RegisterInputs();
	void Render();
	void Cleanups();

	void ToggleMenu();
	void Pop();

	inline HHOOK g_hMouseHook = NULL;
	inline HHOOK g_hKeyboardHook = NULL;
	LRESULT CALLBACK MouseEventHandler(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK KeyboardEventHandler(int nCode, WPARAM wParam, LPARAM lParam);
}