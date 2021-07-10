#pragma once
#include <Windows.h>

class Edit
{
public:
	Edit(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0)
	{
		hWnd = CreateWindowExA(WS_EX_CLIENTEDGE, TEXT("Edit"), text, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, hParent, NULL, NULL, NULL);
	}
	LPCSTR getText()
	{
		GetWindowTextA(hWnd, text, 100);
		return text;
	}
	HWND hWnd = NULL;
	char text[100] = { };
};