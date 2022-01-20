#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"
#include "WindowStyles.h"

class ColorButton
{
	bool checked = false, hovering = false;
	HPEN checkedPen, pen;
	BYTE red, green, blue;
	HBRUSH brush;

	bool empty = false;
	static int height, width;

public:
	ColorButton(HWND hParent, BYTE _red, BYTE _green, BYTE _blue, int x, int y, int width, int height, bool _empty = false);
	~ColorButton();
	void check(bool state);
	bool isChecked();
	void draw(LPDRAWITEMSTRUCT& pDIS);
	void getColor(char* buffer, int size);
	void mouseHovering(bool isHovering);

	HWND hWnd = NULL;
	HMENU hMenu;
};