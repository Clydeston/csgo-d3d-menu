#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "config.h"
#include "drawing.h"
#include "d3dmenu.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(__stdcall * _endScene)(IDirect3DDevice9 * pDevice);
HRESULT __stdcall hookedEndScene(IDirect3DDevice9* o_pDevice);

namespace Hook
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size)
	{
		DWORD oProc;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oProc, &oProc);
	}

	// hook function
	bool Hook(char* src, char* dst, int len)
	{
		if (len < 5) return false;
		DWORD oProc;
		VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
		memset(src, 0x90, len);
		uintptr_t relAddy = (uintptr_t)(dst - src - 5);
		*src = (char)0xE9;
		*(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
		VirtualProtect(src, len, oProc, &oProc);
	}

	// trampoline hook function
	char* TrampHook(char* src, char* dst, unsigned int len)
	{
		if (len < 5) return 0;
		char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(gateway, src, len);
		uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
		*(gateway + len) = (char)0xE9;
		*(uintptr_t*)(gateway + len + 1) = jumpAddy;
		if (Hook(src, dst, len))
		{
			return gateway;
		}
		else return nullptr;
	}

	namespace Dx9
	{
		void** vTable;
		BYTE EndSceneBytes[7]{ 0 };
		IDirect3DDevice9* pDevice;
		_endScene endScene;

		void HookEndScene()
		{
			pDevice = *(IDirect3DDevice9**)(Config::dx9DLL + Offsets::dwppDirect3DDevice9);
			vTable = *reinterpret_cast<void***>(pDevice);
			memcpy(EndSceneBytes, (char*)vTable[42], 7);
			endScene = (_endScene)TrampHook((char*)vTable[42], (char*)hookedEndScene, 7);
		}
		void Unhook()
		{
			Patch((BYTE*)vTable[42], EndSceneBytes, 7);
		}
	}
}

HRESULT __stdcall hookedEndScene(IDirect3DDevice9* o_pDevice)
{
	if (!Hook::Dx9::pDevice)
		Hook::Dx9::pDevice = o_pDevice;
	
	Drawing::pDevice = Hook::Dx9::pDevice;

	Drawing::DrawText("Show Menu (INS)", Config::windowWidth / 2, 50, D3DCOLOR_ARGB(255, 255, 255, 255), DT_CENTER);
	if (Config::bMenuOpen)
	{		
		D3DMenu::Draw();		
	}

	return Hook::Dx9::endScene(o_pDevice);
}
