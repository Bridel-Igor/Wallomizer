#pragma once

#include "Static.h"
#include "Button.h"
#include "BaseCollection.h"

class CollectionItem
{
public:
	CollectionItem(HWND hParent, int x, int y, int width, int height, BaseCollection *collection, HFONT hFont)
	{
		stName = new Static(hParent, "",			x,			y,		width - 90,	height, 0, WS_EX_STATICEDGE);
		stNumber = new Static(hParent, "0",			x+width-90,	y,		50,			height, 0, WS_EX_STATICEDGE);
		btnSettings = new Button(hParent, "...",	x+width-40,	y,		20,			height, 0, WS_EX_STATICEDGE);
		btnDelete = new Button(hParent, "X",		x+width-20,	y,		20,			height, 0, WS_EX_STATICEDGE);
		SendMessage(stName->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
		SendMessage(stNumber->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
		SendMessage(btnSettings->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
		SendMessage(btnDelete->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
		updateInfo(collection);
	}
	~CollectionItem()
	{
		if (stName != nullptr)
			delete stName;
		if (stNumber != nullptr)
			delete stNumber;
		if (btnSettings != nullptr)
			delete btnSettings;
		if (btnDelete != nullptr)
			delete btnDelete;
	}
	void updateInfo(BaseCollection* collection)
	{
		if (collection == nullptr)
			return;
		SetWindowTextA(stName->hWnd, collection->collectionName());
		char c[10];
		_itoa_s(collection->getNumber(), c, 10);
		c[9] = '\0';
		SetWindowTextA(stNumber->hWnd, c);
	}

	Static* stName, * stNumber;
	Button *btnSettings, *btnDelete;
	static const COLORREF fontColor = RGB(200, 200, 200);
	static const COLORREF bkColor = RGB(26, 26, 26);
};