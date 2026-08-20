#include "CollectionItem.h"

#include <Windows.h>

#include "resource.h"
#include "BaseCollection.h"
#include "IWindow.h"
#include "UIColor.h"

HICON CollectionItem::Resources::hIOptions, 
	CollectionItem::Resources::hIOptionsHover, 
	CollectionItem::Resources::hIDelete, 
	CollectionItem::Resources::hIDeleteHover;
HBRUSH CollectionItem::Resources::collItemBkBrush;

std::uint16_t CollectionItem::Resources::refCount = 0;

CollectionItem::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	collItemBkBrush = CreateSolidBrush(UIColor::collectionItemBk);
	hIDelete =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIDeleteHover =		static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIOptions =			static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	hIOptionsHover =	static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS_HOVER),IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
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

CollectionItem::CollectionItem(IComponent* pParent, int _x, int _y, int _width)
	: x(_x), y(_y), width(_width),
	chboEnabled(pParent,	0, 0, 0, 0, 0, BS_NOTIFY | BS_OWNERDRAW),
	stName(pParent, L"",	0, 0, 0, 0),
	purity(pParent,			0, 0, 0, 0),
	stNumber(pParent, "0",	0, 0, 0, 0, SS_CENTER),
	btnSettings(pParent,	0, 0, 0, height, resources.hIOptions, resources.hIOptionsHover),
	btnDelete(pParent,		0, 0, 0, height, resources.hIDelete, resources.hIDeleteHover)
{
	stName.setFont(IWindow::Resources::mainFont);
	stNumber.setFont(IWindow::Resources::mainFont);
	purity.enable(false);
}

void CollectionItem::updateInfo(const BaseCollection& collection)
{
	purity.setPurity(collection.getCAP());
	chboEnabled.setChecked(collection.isEnabled());
	stName.setText(collection.getCollectionName());
	stNumber.setText(std::to_wstring(collection.getWallpaperCount()));
}

void CollectionItem::reposition(int yPos, bool scrollbarVisible)
{
	const int offset = scrollbarVisible * scrollbarOffset;
	MoveWindow(chboEnabled.hWnd(),	x,										y - yPos,	22,									height, FALSE);
	MoveWindow(stName.hWnd(),		x + 23,									y - yPos,	width - height * 3 - 121 - offset,	height, FALSE);
	purity.moveComponent(			x + width - height * 3 - 97 - offset,	y - yPos,	height * 3,							height);
	MoveWindow(stNumber.hWnd(),		x + width - 96 - offset,				y - yPos,	50,									height, FALSE);
	MoveWindow(btnSettings.hWnd(),	x + width - 45 - offset,				y - yPos,	22,									height, FALSE);
	MoveWindow(btnDelete.hWnd(),	x + width - 22 - offset,				y - yPos,	22,									height, FALSE);
}

bool CollectionItem::draw(LPDRAWITEMSTRUCT pDIS)
{
	return chboEnabled.draw(pDIS, resources.collItemBkBrush)
		|| purity.draw(pDIS)
		|| btnSettings.draw(pDIS, false, false, resources.collItemBkBrush, 1, (height - 20) / 2)
		|| btnDelete.draw(pDIS, false, false, resources.collItemBkBrush, 1, (height - 20) / 2);
}
