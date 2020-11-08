#pragma once
#include <iostream>
#include <Windows.h>
#include <cstdint>

namespace Config
{
	extern uintptr_t moduleBase;	
	extern uintptr_t dx9DLL;
	extern HWND hWnd;

	extern bool bMenuOpen;

	extern int windowWidth;
	extern int windowHeight;

	extern bool bVisuals;
	extern bool bVisuals2;
	extern int bChamsType;
}

namespace Offsets
{
	constexpr ::std::ptrdiff_t dwppDirect3DDevice9 = 0xA7050;
}