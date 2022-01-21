#pragma once

#include "IHoverable.h"

class PushButton : public IHoverable
{
public:
	PushButton(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0, COLORREF color1 = RGB(130, 130, 130), COLORREF color2 = RGB(80, 80, 80));
	~PushButton();
	void check(bool state);
	bool isChecked();
	void draw(LPDRAWITEMSTRUCT& pDIS);

private:
	bool checked = false;
	COLORREF checked1, checked2;
	RECT temp = {0};
	HBRUSH color = NULL;
};