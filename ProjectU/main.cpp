#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <filesystem>

#include "Gui/Gui.h"
#include "Core/Globals.h"
#include "Core/Core.h"
#include "Protection/xorstr.h"


int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{

	HANDLE mutex = CreateMutexA(0, FALSE, xorstr_("Global\\$9058432"));

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBoxA(NULL, xorstr_("Can only run once"), "Error", MB_ICONERROR);
		if(mutex)
			CloseHandle(mutex);
		return EXIT_FAILURE;
	}

	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);

	std::filesystem::path programPath(buffer);

	if (!programPath.empty() && programPath.has_root_name()) {
		if (programPath.root_name().string() == xorstr_("C:")) {
			//MessageBox(NULL, xorstr_("C Drive is disallowed"), "Error", MB_ICONERROR);
			//return EXIT_FAILURE;
		}
	}
	else {
		return EXIT_FAILURE;
	}

	Core::Init();

	HANDLE updateThread = (HANDLE)_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(Core::Updates), nullptr, 0u, nullptr);
	HANDLE inputThread = (HANDLE)_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(Core::RegisterInputs), nullptr, 0u, nullptr);
	if(updateThread)
		SetThreadPriority(updateThread, THREAD_PRIORITY_TIME_CRITICAL);
	if(inputThread)
		SetThreadPriority(inputThread, THREAD_PRIORITY_TIME_CRITICAL);

	if(updateThread)
		CloseHandle(updateThread);
	if(inputThread)
		CloseHandle(inputThread);

	while (!Globals::exitFlag) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); Core::Render(); }

	Sleep(150);

	Core::Cleanups();
	if(mutex)
		CloseHandle(mutex);

	return EXIT_SUCCESS;
}