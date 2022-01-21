#include <Windows.h>
#include <CommCtrl.h>

#include "CollectionItem.h"
#include "WindowStyles.h"

int CollectionItem::height = 26;

CollectionItem::CollectionItem(HWND hParent, int _x, int _y, int _width, BaseCollection* collection, HFONT hFont)
	: x(_x), y(_y), width(_width)
{
	chboEnabled = new CheckBox(hParent, "",		0, 0, 0, 0, 0, (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE), BS_NOTIFY | BS_OWNERDRAW);
	stName = new Static(hParent, "",			0, 0, 0, 0, 0);
	purity = new PurityComponent(hParent,		0, 0, 0, 0);
	stNumber = new Static(hParent, "0",			0, 0, 0, 0, SS_CENTER);
	btnSettings = new IconButton(hParent,		0, 0, 0, height, WindowStyles::hIOptions, WindowStyles::hIOptionsHover);
	btnDelete = new IconButton(hParent,			0, 0, 0, height, WindowStyles::hIDelete, WindowStyles::hIDeleteHover);
	SendMessage(stName->hWnd(), WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(stNumber->hWnd(), WM_SETFONT, (LPARAM)hFont, TRUE);
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
	SetWindowTextA(stName->hWnd(), collection->collectionName());
	char c[10];
	_itoa_s(collection->getNumber(), c, 10);
	c[9] = '\0';
	SetWindowTextA(stNumber->hWnd(), c);
}

void CollectionItem::reposition(int yPos, bool sb)
{
	MoveWindow(chboEnabled->hWnd(),	x,										y - yPos,	22,									height, FALSE);
	MoveWindow(stName->hWnd(),		x + 23,									y - yPos,	width - height * 3 - 121 - sb * 18,	height, FALSE);
	purity->moveComponent(			x + width - height * 3 - 97 - sb * 18,	y - yPos,	height * 3,							height);
	MoveWindow(stNumber->hWnd(),	x + width - 96 - sb * 18,				y - yPos,	50,									height, FALSE);
	MoveWindow(btnSettings->hWnd(),	x + width - 45 - sb * 18,				y - yPos,	22,									height, FALSE);
	MoveWindow(btnDelete->hWnd(),	x + width - 22 - sb * 18,				y - yPos,	22,									height, FALSE);
}

bool CollectionItem::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (chboEnabled->draw(pDIS, WindowStyles::collItemBkBrush))
		return true;
	if (purity->draw(pDIS))
		return true;
	if (btnSettings->draw(pDIS, WindowStyles::collItemBkBrush, 1, (height - 20) / 2))
		return true;
	if (btnDelete->draw(pDIS, WindowStyles::collItemBkBrush, 1, (height - 20) / 2))
		return true;
	return false;
}

void CollectionItem::mouseHovering(WPARAM wParam)
{
	chboEnabled->mouseHovering(wParam);
	btnSettings->mouseHovering(wParam);
	btnDelete->mouseHovering(wParam);
}