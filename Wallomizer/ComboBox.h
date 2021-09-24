#pragma once

#include <Windows.h>

class ComboBox
{
public:
	ComboBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, int nOfItems = 0, int lenOfItems = 0, CHAR *items = nullptr, int selectedItem = 0, DWORD additionalStyles = 0)
	{
		hWnd = CreateWindowA("COMBOBOX", text, CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | additionalStyles, x, y, width, height, hParent, NULL, NULL, NULL);

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
	void getTextA(char* buffer, int size)
	{
		GetWindowTextA(hWnd, buffer, size);
	}
	void setTextA(LPCSTR text)
	{
		SetWindowTextA(hWnd, text);
	}
	bool isEmpty()
	{
		char buf[10] = { 0 };
		GetWindowTextA(hWnd, buf, 10);
		return strlen(buf) ? false : true;
	}

	HWND hWnd;
};

/*
Example:
CHAR list[2][10] = {TEXT("item1"), TEXT("item2")};
cbComBox = new ComboBox(Window(), "", x, y, w, h, 2, 10, &TimeUnitsList[0][0]);
*/