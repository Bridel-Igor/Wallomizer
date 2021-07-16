#pragma once
#include "Windows.h"

class ComboBox
{
public:
	ComboBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, int nOfItems, int lenOfItems, CHAR *items, int selectedItem = 0, DWORD additionalStyles = 0)
	{
		hWnd = CreateWindowA("COMBOBOX", text, CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, x, y, width, height, hParent, NULL, NULL,	NULL);

		for (int i = 0; i < nOfItems; i++)
		{
			SendMessage(hWnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(CHAR*)items);
			items += lenOfItems;
		}

		SendMessage(hWnd, CB_SETCURSEL, (WPARAM)selectedItem, (LPARAM)0);
	}
	~ComboBox()
	{
		DestroyWindow(hWnd);
	}
	int getSelectedItem()
	{
		return SendMessage(hWnd, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	}

	HWND hWnd;
};

/*
Example:
CHAR list[2][10] = {TEXT("item1"), TEXT("item2")};
cbComBox = new ComboBox(Window(), "", x, y, w, h, 2, 10, &TimeUnitsList[0][0]);
*/