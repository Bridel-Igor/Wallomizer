#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"

class PushButton
{
	bool checked = false, hovering;
	COLORREF checked1, checked2;
	RECT temp = {0};
	HBRUSH color = NULL;

public:
	PushButton(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0, COLORREF color1 = RGB(130, 130, 130), COLORREF color2 = RGB(80, 80, 80));
	~PushButton();
	void check(bool state);
	bool isChecked();
	void draw(LPDRAWITEMSTRUCT& pDIS);
	void mouseHovering(bool isHovering);

	HWND hWnd = NULL;
	HMENU hMenu;
};
// TODO: make all PushButtons sensible to mouse hovering