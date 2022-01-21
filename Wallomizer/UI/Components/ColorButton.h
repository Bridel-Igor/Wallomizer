#pragma once

#include "IHoverable.h"

class ColorButton : public IHoverable
{
public:
	ColorButton(HWND hParent, BYTE _red, BYTE _green, BYTE _blue, int x, int y, int width, int height, bool _empty = false);
	~ColorButton();
	void check(bool state);
	bool isChecked();
	void draw(LPDRAWITEMSTRUCT& pDIS);
	void getColor(char* buffer, int size);

private:
	bool checked = false;
	bool empty = false;
	static int height, width;
	HPEN checkedPen, pen;
	BYTE red, green, blue;
	HBRUSH brush;
};