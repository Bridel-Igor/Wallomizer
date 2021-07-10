#pragma once
#include <Windows.h>

class Button
{
public:
	Button(HWND hParent, LPCSTR text, int x, int y, int width, int height, HMENU _hMenu, DWORD additionalStyles = 0)
	{
		hMenu = _hMenu;
		hWnd = CreateWindowExA(NULL, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | additionalStyles, x, y, width, height, hParent, hMenu, NULL, NULL);
	}

	HWND hWnd = NULL;
	HMENU hMenu;
};