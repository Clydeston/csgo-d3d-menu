#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "vector.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Drawing
{
	ID3DXLine* LineL;
	ID3DXFont* FontF;
	IDirect3DDevice9* pDevice;

	void DxRect(int x, int y, int w, int h, D3DCOLOR color)
	{
		D3DRECT rect = { x,y, x + w, y + h };
		pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0.0f, 0);
	}

	void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
	{

		if (!LineL)
			D3DXCreateLine(pDevice, &LineL);

		D3DXVECTOR2 Line[2];
		Line[0] = D3DXVECTOR2(x1, y1);
		Line[1] = D3DXVECTOR2(x2, y2);
		LineL->SetWidth(thickness);
		LineL->Draw(Line, 2, color);
	}

	void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color)
	{
		DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
	}

	void DrawText(const char* text, float x, float y, D3DCOLOR color, int textStartPosition)
	{
		RECT rect;

		if (!Drawing::FontF)
			D3DXCreateFont(Drawing::pDevice, 14, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &Drawing::FontF);

		SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
		Drawing::FontF->DrawTextA(NULL, text, -1, &rect, textStartPosition | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

		SetRect(&rect, x, y, x, y);
		Drawing::FontF->DrawTextA(NULL, text, -1, &rect, textStartPosition | DT_NOCLIP, color);
	}
}

