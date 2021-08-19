#pragma once

#include <Windows.h>

#include "HMenuGenerator.h"
#include "WindowStyles.h"

class ColorButton
{
	bool checked = false;
	HPEN checkedPen, pen;
	BYTE red, green, blue;
	HBRUSH brush;
	bool empty = false;
	HPEN emptyPen;

public:
	ColorButton(HWND hParent, BYTE _red, BYTE _green, BYTE _blue, int x, int y, int width, int height, bool _empty=false)
	{
		hMenu = HMenuGenerator::getNewHMenu();
		hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, hParent, hMenu, NULL, NULL);
		empty = _empty;
		red = _red;
		green = _green;
		blue = _blue;
		brush = CreateSolidBrush(RGB(red, green, blue));
		pen = CreatePen(PS_NULL, 0, 0);
		checkedPen = CreatePen(PS_SOLID, 2, red+green+blue>255?RGB(0, 0, 0):RGB(255, 255, 255));
		emptyPen = empty ? CreatePen(PS_SOLID, 3, RGB(255, 0, 0)) : NULL;
	}
	~ColorButton()
	{
		if (empty)
			DeleteObject(emptyPen);
		DeleteObject(pen);
		DeleteObject(checkedPen);
		DeleteObject(brush);
		DestroyWindow(hWnd);
		HMenuGenerator::releaseHMenu(hMenu);
	}
	void check(bool state)
	{
		if (checked != state)
		{
			checked = state;
			InvalidateRect(hWnd, nullptr, true);
		}
	}
	bool isChecked()
	{
		return checked;
	}
	void draw(LPDRAWITEMSTRUCT& pDIS)
	{
		FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
		SelectObject(pDIS->hDC, pen);
		SelectObject(pDIS->hDC, brush);
		RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
		if (empty)
		{
			SelectObject(pDIS->hDC, emptyPen);

			MoveToEx(pDIS->hDC, pDIS->rcItem.left+2, pDIS->rcItem.top+2, NULL);
			LineTo(pDIS->hDC, pDIS->rcItem.right-4, pDIS->rcItem.bottom-4);
		}
		if (checked)
		{
			SelectObject(pDIS->hDC, checkedPen);

			MoveToEx(pDIS->hDC, pDIS->rcItem.left+1, pDIS->rcItem.top+1, NULL);
			LineTo(pDIS->hDC, pDIS->rcItem.left+1, pDIS->rcItem.bottom-1);
			LineTo(pDIS->hDC, pDIS->rcItem.right-1, pDIS->rcItem.bottom-1);
			LineTo(pDIS->hDC, pDIS->rcItem.right-1, pDIS->rcItem.top+1);
			LineTo(pDIS->hDC, pDIS->rcItem.left+1, pDIS->rcItem.top+1);

			RECT tmp; 
			tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
			tmp.top = pDIS->rcItem.top, tmp.bottom = tmp.top + 1;
			FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
			tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
			tmp.top = pDIS->rcItem.top, tmp.bottom = tmp.top + 1;
			FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
			tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
			tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
			FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
			tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
			tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
			FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);

			MoveToEx(pDIS->hDC, (pDIS->rcItem.right-pDIS->rcItem.left)/2-5, (pDIS->rcItem.bottom-pDIS->rcItem.top)/2-3, NULL);
			LineTo(pDIS->hDC, (pDIS->rcItem.right - pDIS->rcItem.left) / 2 - 2, (pDIS->rcItem.bottom - pDIS->rcItem.top) / 2 + 6);
			LineTo(pDIS->hDC, (pDIS->rcItem.right - pDIS->rcItem.left) / 2 + 6, (pDIS->rcItem.bottom - pDIS->rcItem.top) / 2 - 6);
		}
	}
	void getColor(char* buffer, int size)
	{
		char c[3] = { 0 };
		_itoa_s(red, c, 16);
		if (strlen(c) == 1)
		{
			c[1] = c[0];
			c[0] = '0';
			c[2] = '\0';
		}
		strcat_s(buffer, size, c);
		_itoa_s(green, c, 16);
		if (strlen(c) == 1)
		{
			c[1] = c[0];
			c[0] = '0';
			c[2] = '\0';
		}
		strcat_s(buffer, size, c);
		_itoa_s(blue, c, 16);
		if (strlen(c) == 1)
		{
			c[1] = c[0];
			c[0] = '0';
			c[2] = '\0';
		}
		strcat_s(buffer, size, c);
	}

	HWND hWnd = NULL;
	HMENU hMenu;
};