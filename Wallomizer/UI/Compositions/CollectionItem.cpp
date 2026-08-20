#include "CollectionItem.h"

#include <Windows.h>

#include "resource.h"
#include "BaseCollection.h"
#include "IWindow.h"
#include "UIColor.h"
#include "GraphicsUtils.h"

CollectionItem::Resources::Resources()
{
	brushBk =			CreateSolidBrush(UIColor::collectionItemBk);
	iconDelete =		GraphicsUtils::loadIcon(IDI_DELETE);
	iconDeleteHover =	GraphicsUtils::loadIcon(IDI_DELETE_HOVER);
	iconOptions =		GraphicsUtils::loadIcon(IDI_OPTIONS);
	iconOptionsHover =	GraphicsUtils::loadIcon(IDI_OPTIONS_HOVER);
}

CollectionItem::Resources::~Resources()
{
	DeleteObject(brushBk);
	DestroyIcon(iconDelete);
	DestroyIcon(iconDeleteHover);
	DestroyIcon(iconOptions);
	DestroyIcon(iconOptionsHover);
}

CollectionItem::CollectionItem(IComponent* pParent, int _x, int _y, int _width)
	: x(_x), y(_y), width(_width),
	chboEnabled(pParent,	0, 0, 0, 0, 0, BS_NOTIFY | BS_OWNERDRAW),
	stName(pParent, L"",	0, 0, 0, 0),
	purity(pParent,			0, 0, 0, 0),
	stNumber(pParent, "0",	0, 0, 0, 0, SS_CENTER),
	btnSettings(pParent,	0, 0, 0, height, m_resources.get().iconOptions, m_resources.get().iconOptionsHover),
	btnDelete(pParent,		0, 0, 0, height, m_resources.get().iconDelete, m_resources.get().iconDeleteHover)
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

bool CollectionItem::draw(LPDRAWITEMSTRUCT drawItem)
{
	return chboEnabled.draw(drawItem, m_resources.get().brushBk)
		|| purity.draw(drawItem)
		|| btnSettings.draw(drawItem, false, false, m_resources.get().brushBk, 1, (height - 20) / 2)
		|| btnDelete.draw(drawItem, false, false, m_resources.get().brushBk, 1, (height - 20) / 2);
}

LRESULT CollectionItem::handleColor(HWND hWnd, HDC hdc) const
{
	if (hWnd == stNumber.hWnd() || hWnd == stName.hWnd())
	{
		if (chboEnabled.isChecked())
			SetTextColor(hdc, UIColor::collectionItemText);
		else
			SetTextColor(hdc, UIColor::collectionItemTextInactive);
		SetBkColor(hdc, UIColor::collectionItemBk);
		return reinterpret_cast<LRESULT>(m_resources.get().brushBk);
	}
	if (hWnd == chboEnabled.hWnd() ||
		hWnd == btnDelete.hWnd() ||
		hWnd == btnSettings.hWnd())
	{
		SetTextColor(hdc, UIColor::collectionItemText);
		SetBkColor(hdc, UIColor::collectionItemBk);
		return reinterpret_cast<LRESULT>(m_resources.get().brushBk);
	}
	return 0;
}

bool CollectionItem::toggle() noexcept
{
	chboEnabled.toggle();
	return chboEnabled.isChecked();
}
