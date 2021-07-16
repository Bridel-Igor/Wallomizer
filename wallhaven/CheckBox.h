#pragma once
#include <Windows.h>

class CheckBox
{
public:
	CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HMENU _hMenu, HINSTANCE hInstance, bool isChecked = false)
	{
		hMenu = _hMenu;
		hWnd = CreateWindowExA(NULL, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_LEFTTEXT, x, y, width, height, hParent, hMenu, hInstance, NULL);
		if (isChecked)
			check();
	}
	~CheckBox()
	{
		DestroyWindow(hWnd);
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