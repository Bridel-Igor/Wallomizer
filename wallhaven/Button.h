#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"

class Button
{
public:
	Button(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0)
	{
		hMenu = HMenuGenerator::getNewHMenu();
		hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | additionalStyles, x, y, width, height, hParent, hMenu, NULL, NULL);
	}
	~Button()
	{
		DestroyWindow(hWnd);
		HMenuGenerator::releaseHMenu(hMenu);
	}

	HWND hWnd = NULL;
	HMENU hMenu;
};