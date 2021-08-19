#include "CollectionItem.h"

CollectionItem::CollectionItem(HWND hParent, int _x, int _y, int _width, int _height, BaseCollection* collection, HFONT hFont)
	: x(_x), y(_y), width(_width), height(_height)
{
	chboEnabled = new CheckBox(hParent, "", x, y, 20, height, 0, (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE), 0, WS_EX_STATICEDGE);
	stName = new Static(hParent, "", x + 20, y, width - 110, height, 0, WS_EX_STATICEDGE);
	stNumber = new Static(hParent, "0", x + width - 90, y, 50, height, 0, WS_EX_STATICEDGE);
	btnSettings = new Button(hParent, "...", x + width - 40, y, 20, height, 0, WS_EX_STATICEDGE);
	btnDelete = new Button(hParent, "X", x + width - 20, y, 20, height, 0, WS_EX_STATICEDGE);
	SendMessage(stName->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(stNumber->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(btnSettings->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(btnDelete->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	updateInfo(collection);
}

CollectionItem::~CollectionItem()
{
	if (chboEnabled != nullptr)
		delete chboEnabled;
	if (stName != nullptr)
		delete stName;
	if (stNumber != nullptr)
		delete stNumber;
	if (btnSettings != nullptr)
		delete btnSettings;
	if (btnDelete != nullptr)
		delete btnDelete;
}

void CollectionItem::updateInfo(BaseCollection* collection)
{
	if (collection == nullptr)
		return;
	chboEnabled->setChecked(collection->isEnabled);
	SetWindowTextA(stName->hWnd, collection->collectionName());
	char c[10];
	_itoa_s(collection->getNumber(), c, 10);
	c[9] = '\0';
	SetWindowTextA(stNumber->hWnd, c);
}

void CollectionItem::scroll(int yPos)
{
	MoveWindow(chboEnabled->hWnd, x, y - yPos, 20, height, FALSE);
	MoveWindow(stName->hWnd, x + 20, y - yPos, width - 110, height, FALSE);
	MoveWindow(stNumber->hWnd, x + width - 90, y - yPos, 50, height, FALSE);
	MoveWindow(btnSettings->hWnd, x + width - 40, y - yPos, 20, height, FALSE);
	MoveWindow(btnDelete->hWnd, x + width - 20, y - yPos, 20, height, FALSE);
}