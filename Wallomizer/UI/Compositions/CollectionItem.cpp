#include <Windows.h>
#include <CommCtrl.h>

#include "CollectionItem.h"
#include "resource.h"

HICON CollectionItem::Resources::hIOptions, CollectionItem::Resources::hIOptionsHover, 
	CollectionItem::Resources::hIDelete, CollectionItem::Resources::hIDeleteHover;
HBRUSH CollectionItem::Resources::collItemBkBrush;
COLORREF CollectionItem::Resources::collItemFontColor = RGB(200, 200, 200);
COLORREF CollectionItem::Resources::collItemBkColor = RGB(36, 36, 36);

unsigned char CollectionItem::Resources::refCount = 0;

CollectionItem::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	collItemBkBrush = CreateSolidBrush(collItemBkColor);
	hIDelete =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIDeleteHover =		(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptions =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptionsHover =	(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

CollectionItem::Resources::~Resources()
{
	if (--refCount) // Destroying icons only if this is the last player destroying
		return;
	DeleteObject(collItemBkBrush);
	DestroyIcon(hIDelete);
	DestroyIcon(hIDeleteHover);
	DestroyIcon(hIOptions);
	DestroyIcon(hIOptionsHover);
}

CollectionItem::CollectionItem(HWND hParent, int _x, int _y, int _width, BaseCollection* pCollection, HFONT hFont)
	: x(_x), y(_y), width(_width), m_pCollection(pCollection),
	chboEnabled(hParent,	0, 0, 0, 0, 0, BS_NOTIFY | BS_OWNERDRAW),
	stName(hParent, L"",	0, 0, 0, 0, 0),
	purity(hParent,			0, 0, 0, 0),
	stNumber(hParent, "0",	0, 0, 0, 0, SS_CENTER),
	btnSettings(hParent,	0, 0, 0, height, resources.hIOptions, resources.hIOptionsHover),
	btnDelete(hParent,		0, 0, 0, height, resources.hIDelete, resources.hIDeleteHover)
{
	SendMessage(stName.hWnd(), WM_SETFONT, (LPARAM)hFont, TRUE);
	SendMessage(stNumber.hWnd(), WM_SETFONT, (LPARAM)hFont, TRUE);
	updateInfo(m_pCollection);
}

void CollectionItem::updateInfo(BaseCollection* collection)
{
	if (collection == nullptr)
		return;
	purity.setPurity(collection->getCAP());
	chboEnabled.setChecked(collection->isEnabled());
	wchar_t text[255] { 0 };
	collection->getCollectionName(text, 255);
	SetWindowTextW(stName.hWnd(), text);
	char c[10];
	_itoa_s(collection->getNumber(), c, 10);
	c[9] = '\0';
	SetWindowTextA(stNumber.hWnd(), c);
}

void CollectionItem::reposition(int yPos, bool sb)
{
	MoveWindow(chboEnabled.hWnd(),	x,										y - yPos,	22,									height, FALSE);
	MoveWindow(stName.hWnd(),		x + 23,									y - yPos,	width - height * 3 - 121 - sb * 18,	height, FALSE);
	purity.moveComponent(			x + width - height * 3 - 97 - sb * 18,	y - yPos,	height * 3,							height);
	MoveWindow(stNumber.hWnd(),		x + width - 96 - sb * 18,				y - yPos,	50,									height, FALSE);
	MoveWindow(btnSettings.hWnd(),	x + width - 45 - sb * 18,				y - yPos,	22,									height, FALSE);
	MoveWindow(btnDelete.hWnd(),	x + width - 22 - sb * 18,				y - yPos,	22,									height, FALSE);
}

bool CollectionItem::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (chboEnabled.draw(pDIS, resources.collItemBkBrush))
		return true;
	if (purity.draw(pDIS))
		return true;
	if (btnSettings.draw(pDIS, resources.collItemBkBrush, 1, (height - 20) / 2))
		return true;
	if (btnDelete.draw(pDIS, resources.collItemBkBrush, 1, (height - 20) / 2))
		return true;
	return false;
}

void CollectionItem::mouseHovering(WPARAM wParam)
{
	chboEnabled.mouseHovering(wParam);
	btnSettings.mouseHovering(wParam);
	btnDelete.mouseHovering(wParam);
}