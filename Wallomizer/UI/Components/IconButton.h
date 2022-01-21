#pragma once

#include "IHoverable.h"

class IconButton : public IHoverable
{
public:
	IconButton(HWND hParent, int x, int y, int width, int height, HICON& _hIcon, HICON& _hIconHover, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~IconButton();
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd, int x = 0, int y = 0);

private:
	HICON hIcon, hIconHover;
};