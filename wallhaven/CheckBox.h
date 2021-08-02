#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"

class CheckBox
{
public:
	CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HINSTANCE hInstance, bool isChecked = false, DWORD additionalStyles = 0, DWORD additionalExStyles = 0)
	{
		hMenu = HMenuGenerator::getNewHMenu();
		hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | additionalStyles, x, y, width, height, hParent, hMenu, hInstance, NULL);
		if (isChecked)
			check();
	}
	~CheckBox()
	{
		DestroyWindow(hWnd);
		HMenuGenerator::releaseHMenu(hMenu);
	}
	void setChecked(bool state)
	{
		SendMessageA(hWnd, BM_SETCHECK, (WPARAM)state, NULL);
	}
	void check()
	{
		SendMessageA(hWnd, BM_SETCHECK, BST_CHECKED, NULL);
	}
	void uncheck()
	{
		SendMessageA(hWnd, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	bool isChecked()
	{
		return SendMessageA(hWnd, BM_GETCHECK, NULL, NULL);
	}

	HWND hWnd = NULL;
	HMENU hMenu;
};