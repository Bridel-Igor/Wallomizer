#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"

class IconButton
{
public:
	IconButton(HWND hParent, int x, int y, int width, int height, HICON& _hIcon, HICON& _hIconHover, DWORD additionalStyles = 0, DWORD additionalExStyles = 0)
	{
		hMenu = HMenuGenerator::getNewHMenu();
		hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, hMenu, NULL, NULL);
		hovering = false;
		hIcon = _hIcon;
		hIconHover = _hIconHover;
	}
	~IconButton()
	{
		DestroyWindow(hWnd);
		HMenuGenerator::releaseHMenu(hMenu);
	}
	void mouseHovering(bool isHovering)
	{
		if (hovering != isHovering)
		{
			hovering = isHovering;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd, int x = 0, int y = 0)
	{
		if (pDIS->hwndItem != hWnd)
			return false;
		FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);
		DrawIconEx(pDIS->hDC, x, y, hovering ? hIconHover : hIcon, 0, 0, 0, NULL, DI_NORMAL);
		return true;
	}

	HWND hWnd = NULL;
	HMENU hMenu;

private:
	bool hovering;
	HICON hIcon, hIconHover;
};