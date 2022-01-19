#include <Windows.h>
#include <CommCtrl.h>

#include "CollectionItem.h"
#include "WindowStyles.h"

CollectionItem::CollectionItem(HWND hParent, int _x, int _y, int _width, int _height, BaseCollection* collection, HFONT hFont)
	: x(_x), y(_y), width(_width), height(_height)
{
	chboEnabled = new CheckBox(hParent, "",		0, 0, 0, 0, 0, (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE), BS_NOTIFY | BS_OWNERDRAW);
	stName = new Static(hParent, "",			0, 0, 0, 0, 0);
	purity = new PurityComponent(hParent,		0, 0, 0, 0);
	stNumber = new Static(hParent, "0",			0, 0, 0, 0, SS_CENTER);
	btnSettings = new Button(hParent, "...",	0, 0, 0, 0, BS_OWNERDRAW);
	btnDelete = new Button(hParent, "",			0, 0, 0, 0, BS_OWNERDRAW);
	SendMessage(stName->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(stNumber->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(btnSettings->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(btnDelete->hWnd, WM_SETFONT, (LPARAM)hFont, TRUE);
	updateInfo(collection);
}

CollectionItem::~CollectionItem()
{
	delete chboEnabled;
	delete stName;
	delete purity;
	delete stNumber;
	delete btnSettings;
	delete btnDelete;
}

void CollectionItem::updateInfo(BaseCollection* collection)
{
	if (collection == nullptr)
		return;
	purity->setPurity(collection->getCAP());
	chboEnabled->setChecked(collection->isEnabled);
	SetWindowTextA(stName->hWnd, collection->collectionName());
	char c[10];
	_itoa_s(collection->getNumber(), c, 10);
	c[9] = '\0';
	SetWindowTextA(stNumber->hWnd, c);
}

void CollectionItem::reposition(int yPos, bool sb)
{
	MoveWindow(chboEnabled->hWnd,	x,										y - yPos,	22,									height, FALSE);
	MoveWindow(stName->hWnd,		x + 23,									y - yPos,	width - height * 3 - 121 - sb * 18,	height, FALSE);
	purity->moveComponent(			x + width - height * 3 - 97 - sb * 18,	y - yPos,	height * 3,							height);
	MoveWindow(stNumber->hWnd,		x + width - 96 - sb * 18,				y - yPos,	50,									height, FALSE);
	MoveWindow(btnSettings->hWnd,	x + width - 45 - sb * 18,				y - yPos,	22,									height, FALSE);
	MoveWindow(btnDelete->hWnd,		x + width - 22 - sb * 18,				y - yPos,	22,									height, FALSE);
}

bool CollectionItem::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (chboEnabled->draw(pDIS, WindowStyles::collItemBkBrush))
		return true;
	if (purity->draw(pDIS))
		return true;
	if (pDIS->hwndItem == btnSettings->hWnd)
	{
		FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::collItemBkBrush);
		DrawIconEx(pDIS->hDC, 1, (height - 20) / 2, WindowStyles::hIOptions, 0, 0, 0, NULL, DI_NORMAL);
		return true;
	}
	if (pDIS->hwndItem == btnDelete->hWnd)
	{
		FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::collItemBkBrush);
		DrawIconEx(pDIS->hDC, 1, (height - 20) / 2, WindowStyles::hIDelete, 0, 0, 0, NULL, DI_NORMAL);
		return true;
	}
	return false;
}

bool CollectionItem::notify(HWND hWnd)
{
	chboEnabled->mouseHovering(hWnd == chboEnabled->hWnd);
	return true;
}