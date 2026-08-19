#include "MainWindow.h"

#include <algorithm>

#include "LocalCollection.h"
#include "UserCollection.h"
#include "SearchCollection.h"
#include "App.h"
#include "TrayWindow.h"
#include "SettingsWindow.h"
#include "AddCollectionWindow.h"
#include "SetLocalCollectionWindow.h"
#include "SetUserCollectionWindow.h"
#include "SetSearchCollectionWindow.h"
#include "UIColor.h"

MainWindow::MainWindow(App& app) :
	IWindow(nullptr, "Wallomizer", "Main Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, width, height),
	m_app(app),
	bkBrush(CreateSolidBrush(UIColor::collectionPanelBk)),
	stCollections		(this, "Collections:",			20,		10,		100,	20),
	btnAdd				(this, "Add collection..",		530,	10,		100,	20),
	collectionsPanel	(this, "CollectionPanelClass",	panelX,		panelY,		panelWidth, panelHeight, bkBrush),
	stEmpty				(&collectionsPanel, "Collection list is empty. Click \"Add collection..\" button to add one.", 
														5,		0,		480,	20),
	btnSettings			(this, "Settings",				10,		450,	95,		20),
	player				(this,							220,	450,
														430,	450,	100,	20, m_app.getAppState(), m_app.getTimer(), m_app.getWallpaperManager())
{
	centerWindow(GetDesktopWindow());
	updateCollectionItems();
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

MainWindow::~MainWindow() noexcept
{
	DeleteObject(bkBrush);
}

LRESULT MainWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (btnAdd.isClicked(wParam))
		{
			AddCollectionWindow addCollectionWindow(this, m_app.getWinUtils(), m_app.getSettings(), m_app.getCollectionManager());
			addCollectionWindow.windowLoop();
			updateCollectionItems();
			return 0;
		}
		if (player.click(wParam))
			return 0;
		if (btnSettings.isClicked(wParam))
		{
			SettingsWindow settingsWindow(this, m_app.getWinUtils(), m_app.getSettings());
			settingsWindow.windowLoop();
			Player::redrawPlayers();
			return 0;
		}
		int i = 0;
		for (auto& collectionItem : collectionItems)
		{
			if (collectionItem.btnSettings.isClicked(wParam))
			{
				BaseCollection& collection = m_app.getCollectionManager().getCollection(i);
				switch (collection.getType())
				{
				case CollectionType::local:
				{
					SetLocalCollectionWindow setLocalCollectionWindow(this, m_app.getWinUtils(), static_cast<LocalCollection&>(collection));
					setLocalCollectionWindow.windowLoop();
					if (setLocalCollectionWindow.isOk())
					{
						m_app.getCollectionManager().saveSettings();
						m_app.getCollectionManager().recountWallpapers();
					}
					break;
				}
				case CollectionType::user:
				{
					SetUserCollectionWindow setUserCollectionWindow(this, m_app.getSettings(), static_cast<UserCollection&>(collection));
					setUserCollectionWindow.windowLoop();
					if (setUserCollectionWindow.isOk())
					{
						m_app.getCollectionManager().saveSettings();
						m_app.getCollectionManager().recountWallpapers();
					}
					break;
				}
				case CollectionType::search:
				{
					SetSearchCollectionWindow setSearchCollectionWindow(this, static_cast<SearchCollection&>(collection));
					setSearchCollectionWindow.windowLoop();
					if (setSearchCollectionWindow.isOk())
					{
						m_app.getCollectionManager().saveSettings();
						m_app.getCollectionManager().recountWallpapers();
					}
					break;
				}
				default:
					break;
				}
				updateCollectionItems();
				return 0;
			}
			if (collectionItem.btnDelete.isClicked(wParam))
			{
				m_app.getCollectionManager().eraseCollection(i);
				updateCollectionItems();
				return 0;
			}
			if (collectionItem.chboEnabled.isClicked(wParam))
			{
				collectionItem.chboEnabled.toggle();
				m_app.getCollectionManager().enableCollection(i, collectionItem.chboEnabled.isChecked());
				updateCollectionItems();
				return 0;
			}
			i++;
		}
		break;
	}

	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			SendMessage(collectionsPanel.hWnd(), WM_VSCROLL, SB_LINEUP, 0L);
		else
			SendMessage(collectionsPanel.hWnd(), WM_VSCROLL, SB_LINEDOWN, 0L);
		break;
	}

	case WM_VSCROLL:
	{
		int yDelta;
		int yNewPos;

		switch (LOWORD(wParam))
		{
		case SB_PAGEUP:
			yNewPos = yCurrentScroll - 60;
			break;
		case SB_PAGEDOWN:
			yNewPos = yCurrentScroll + 60;
			break;
		case SB_LINEUP:
			yNewPos = yCurrentScroll - 10;
			break;
		case SB_LINEDOWN:
			yNewPos = yCurrentScroll + 10;
			break;
		case SB_THUMBTRACK:
			yNewPos = HIWORD(wParam);
			break;
		default:
			yNewPos = yCurrentScroll;
		}

		yNewPos = std::max(0, yNewPos);
		yNewPos = std::min(yMaxScroll, yNewPos);

		if (yNewPos == yCurrentScroll)
			break;

		yDelta = yNewPos - yCurrentScroll;
		yCurrentScroll = yNewPos;

		updateScroll();
		for (auto& p : collectionItems) // placing according to the scrollbar
			p.reposition(yCurrentScroll, scrollBarIsVisible);

		ScrollWindowEx(collectionsPanel.hWnd(), 0, -yDelta, nullptr, nullptr, nullptr, nullptr, SW_INVALIDATE);
		UpdateWindow(collectionsPanel.hWnd());
		break;
	}

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player.draw(pDIS))
			return TRUE;
		for (auto& item : collectionItems)
			if (item.draw(pDIS))
				return TRUE;
		break;
	}

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		HWND hWnd = (HWND)lParam;
		HDC hdc = (HDC)wParam;
		for (auto& item : collectionItems)
		{
			if (hWnd == item.stNumber.hWnd() || hWnd == item.stName.hWnd())
			{
				if (item.chboEnabled.isChecked())
					SetTextColor(hdc, UIColor::collectionItemText);
				else
					SetTextColor(hdc, UIColor::collectionItemTextInactive);
				SetBkColor(hdc, UIColor::collectionItemBk);
				return (LRESULT)CollectionItem::Resources::collItemBkBrush;
			}
			if (hWnd == item.chboEnabled.hWnd() ||
				hWnd == item.btnDelete.hWnd() ||
				hWnd == item.btnSettings.hWnd())
			{
				SetTextColor(hdc, UIColor::collectionItemText);
				SetBkColor(hdc, UIColor::collectionItemBk);
				return (LRESULT)CollectionItem::Resources::collItemBkBrush;
			}
		}
		if (hWnd == stEmpty.hWnd())
		{
			SetTextColor(hdc, UIColor::collectionItemText);
			SetBkMode(hdc, TRANSPARENT);
			return (LRESULT)bkBrush;
		}
		if (hWnd == collectionsPanel.hWnd())
		{
			SetBkColor(hdc, UIColor::collectionPanelBk);
			return (LRESULT)bkBrush;
		}
		break;
	}
	}

	return RESULT_DEFAULT;
}

void MainWindow::updateCollectionItems()
{
	const std::size_t collectionCount = m_app.getCollectionManager().getCollectionCount();
	ShowWindow(stEmpty.hWnd(), collectionCount ? SW_HIDE : SW_SHOW);

	// delete if more then needed
	while (collectionItems.size() > collectionCount)
		collectionItems.pop_back();

	// create if less then needed
	for (std::size_t i = collectionItems.size(); i < collectionCount; i++)
		collectionItems.emplace_back(&collectionsPanel, 0, static_cast<int>(i) * (CollectionItem::height + 1), panelWidth);

	updateScroll();
	std::size_t i = 0;
	for (auto& collectionItem : collectionItems) // placing according to the scrollbar
	{
		collectionItem.updateInfo(m_app.getCollectionManager().getCollection(i++));
		collectionItem.reposition(yCurrentScroll, scrollBarIsVisible);
	}

	InvalidateRect(hWnd(), nullptr, FALSE);
}

void MainWindow::updateScroll()
{
	int itemListHeight = (int)collectionItems.size() * (CollectionItem::height + 1);
	yMaxScroll = std::max(itemListHeight - panelHeight, 0);
	yCurrentScroll = std::min(yCurrentScroll, yMaxScroll);
	yCurrentScroll = yCurrentScroll < 0 ? 0 : yCurrentScroll;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = itemListHeight;
	si.nPage = panelHeight;
	si.nPos = yCurrentScroll;

	scrollBarIsVisible = itemListHeight > panelHeight;
	SetScrollInfo(collectionsPanel.hWnd(), SB_VERT, &si, TRUE);
	ShowScrollBar(collectionsPanel.hWnd(), SB_VERT, scrollBarIsVisible);
	EnableScrollBar(collectionsPanel.hWnd(), SB_VERT, itemListHeight <= panelHeight ? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
}
