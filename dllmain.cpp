#include <Windows.h>
#include "config.h"
#include "hook.h"

HWND hWnd = FindWindowA(0, "Counter-Strike: Global Offensive");

void GetWindowSize()
{
	RECT size;
	GetWindowRect(GetForegroundWindow(), &size);
	Config::windowWidth = size.right - size.left;
	Config::windowHeight = size.bottom - size.top;

	Config::windowHeight -= 29;
	Config::windowWidth -= 5;
}

void GetInfo()
{
	Config::moduleBase = (uintptr_t)GetModuleHandle("client.dll"); 
	Config::dx9DLL = (uintptr_t)GetModuleHandle("shaderapidx9.dll");
	Config::hWnd = hWnd;
	GetWindowSize();
}

DWORD WINAPI Thread(HMODULE hModule)
{
	FILE* f;
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);
	
	// getting necessary values
	GetInfo();
	// endscene hook
	Hook::Dx9::HookEndScene();

	// hack loop
	while (true)
	{		
		std::cout << Config::bVisuals << std::endl;
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			Config::bMenuOpen = !Config::bMenuOpen;
		}

		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}
	}

	Hook::Dx9::Unhook();
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = nullptr;
		hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Thread, hModule, 0, nullptr);

		if (hThread)
		{
			CloseHandle(Thread);
		}
	}
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

