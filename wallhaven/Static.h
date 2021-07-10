#pragma once
#include <Windows.h>

class Static
{
public:
	Static(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0)
	{
		hWnd = CreateWindowExA(WS_EX_TRANSPARENT, TEXT("Static"), text, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, hParent, NULL, NULL, NULL);
	}

	HWND hWnd = NULL;
};