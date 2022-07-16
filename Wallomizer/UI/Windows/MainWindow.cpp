#include <thread>

#include "MainWindow.h"
#include "Settings.h"
#include "AddCollectionWindow.h"
#include "SettingsWindow.h"
#include "TrayWindow.h"
#include "Delay.h"

MainWindow* MainWindow::s_pMainWindow = nullptr;

MainWindow::MainWindow(CollectionManager* pCollectionManager) :
	IWindow("Wallomizer", "Main Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_pCollectionManager(pCollectionManager),
	stCollections		(this, "Collections:",			20,		10,		100,	20),
	btnAdd				(this, "Add collection..",		530,	10,		100,	20),
	fX(10), fY(40), fWidth(width - 20), fHeight(400), bkColor(RGB(15, 15, 15)), bkBrush(CreateSolidBrush(bkColor)),
	collectionsPanel	(this, "CollectionPanelClass",	fX,		fY,		fWidth, fHeight, bkBrush),
	stEmpty				(&collectionsPanel, "Collection list is empty. Click \"Add collection..\" button to add one.", 
														5,		0,		480,	20),
	btnSettings			(this, "Settings",				10,		450,	95,		20),
	player				(this,							250,	450,
														400,	450,	100,	20, m_pCollectionManager)
{
	s_pMainWindow = this;
	while (!m_pCollectionManager->isReady())
		Sleep(50);
	centerWindow(GetDesktopWindow());
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)IWindow::Resources::mainFont);
	player.updateTimer(true);
	updateCollectionItems();
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

MainWindow::~MainWindow()
{
	s_pMainWindow = nullptr;
	ShowWindow(hWnd(), SW_HIDE);
	destroyCollectionItems();
	DeleteObject(bkBrush);
}

LRESULT MainWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		if (wParam == TRUE)
		{
			player.updateTimer(true);
			player.redrawPlayers();
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player.draw(pDIS))
			return TRUE;
		for (auto& item : collectionItems)
			if (item.draw(pDIS))
				return TRUE;
	}
	return 0;

	case WM_COMMAND:
	{
		if (btnAdd.isClicked(wParam))
		{
			AddCollectionWindow addCollectionWindow(hWnd(), m_pCollectionManager);
			addCollectionWindow.windowLoop();
			return 0;
		}
		if (player.click(wParam))
			return 0;
		if (btnSettings.isClicked(wParam))
		{
			SettingsWindow settingsWindow(hWnd());
			settingsWindow.windowLoop();
			Settings::saveSettings();
			return 0;
		}
		int i = 0;
		for (auto& collectionItem : collectionItems)
		{
			if (collectionItem.btnSettings.isClicked(wParam))
			{
				m_pCollectionManager->m_pCollections[i]->openCollectionSettingsWindow(hWnd());
				return 0;
			}
			if (collectionItem.btnDelete.isClicked(wParam))
			{
				m_pCollectionManager->eraseCollection(i);
				updateCollectionItems();
				InvalidateRect(collectionsPanel.hWnd(), nullptr, TRUE);
				return 0;
			}
			if (collectionItem.chboEnabled.isClicked(wParam))
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					collectionItem.chboEnabled.click();
					m_pCollectionManager->m_pCollections[i]->setEnabled(collectionItem.chboEnabled.isChecked());
					m_pCollectionManager->reloadSettings();
					return 0;
				}
			}
			i++;
		}
	}
	return 0;

	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			SendMessage(collectionsPanel.hWnd(), WM_VSCROLL, SB_LINEUP, 0L);
		else
			SendMessage(collectionsPanel.hWnd(), WM_VSCROLL, SB_LINEDOWN, 0L);
	}
	return 0;

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

		yNewPos = max(0, yNewPos);
		yNewPos = min(yMaxScroll, yNewPos);

		if (yNewPos == yCurrentScroll)
			break;

		yDelta = yNewPos - yCurrentScroll;
		yCurrentScroll = yNewPos;

		updateScroll();
		for (auto& p : collectionItems) // placing according to the scrollbar
			p.reposition(yCurrentScroll, scrollBarIsVisible);

		ScrollWindowEx(collectionsPanel.hWnd(), 0, -yDelta, nullptr, nullptr, (HRGN)NULL, (PRECT)NULL, SW_INVALIDATE);
		UpdateWindow(collectionsPanel.hWnd());
	}
	return 0;

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
					SetTextColor(hdc, CollectionItem::Resources::collItemFontColor);
				else
					SetTextColor(hdc, RGB(80, 80, 80));
				SetBkColor(hdc, CollectionItem::Resources::collItemBkColor);
				return (LRESULT)CollectionItem::Resources::collItemBkBrush;
			}
			if (hWnd == item.chboEnabled.hWnd() ||
				hWnd == item.btnDelete.hWnd() ||
				hWnd == item.btnSettings.hWnd())
			{
				SetTextColor(hdc, CollectionItem::Resources::collItemFontColor);
				SetBkColor(hdc, CollectionItem::Resources::collItemBkColor);
				return (LRESULT)CollectionItem::Resources::collItemBkBrush;
			}
		}
		if (hWnd == stEmpty.hWnd())
		{
			SetTextColor(hdc, CollectionItem::Resources::collItemFontColor);
			SetBkMode(hdc, TRANSPARENT);
			return (LRESULT)bkBrush;
		}
		if (hWnd == collectionsPanel.hWnd())
		{
			SetBkColor(hdc, bkColor);
			return (LRESULT)bkBrush;
		}
		// Don't return so IWindow could process another components
	}
	}
	return RESULT_DEFAULT;
}

void MainWindow::updateCollectionItems()
{
	ShowWindow(stEmpty.hWnd(), SW_HIDE);

	size_t i;
	for (i = m_pCollectionManager->m_pCollections.size(); i < collectionItems.size(); i++) //deleting excess items
		collectionItems.pop_back();

	i = 0;
	for (auto& collectionItem : collectionItems) // updating those which won't be created
		collectionItem.updateInfo(m_pCollectionManager->m_pCollections[i++]);

	for (i = collectionItems.size(); i < m_pCollectionManager->m_pCollections.size(); i++) // creation
		if (m_pCollectionManager->m_pCollections[i] != nullptr)
			collectionItems.emplace_back(&collectionsPanel, 0, (int)(i * (CollectionItem::height + 1)), fWidth, m_pCollectionManager->m_pCollections[i], IWindow::Resources::mainFont);

	updateScroll();
	for (auto& collectionItem : collectionItems) // placing according to the scrollbar
		collectionItem.reposition(yCurrentScroll, scrollBarIsVisible);

	if (collectionItems.size() == 0)
		ShowWindow(stEmpty.hWnd(), SW_SHOW);

	InvalidateRect(hWnd(), NULL, FALSE);
}

void MainWindow::destroyCollectionItems()
{
	collectionItems.clear();
	updateScroll();
}

void MainWindow::updateScroll()
{
	int itemListHeight = (int)collectionItems.size() * (CollectionItem::height + 1);
	yMaxScroll = max(itemListHeight - fHeight, 0);
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	yCurrentScroll = yCurrentScroll < 0 ? 0 : yCurrentScroll;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = itemListHeight;
	si.nPage = fHeight;
	si.nPos = yCurrentScroll;

	scrollBarIsVisible = itemListHeight <= fHeight ? FALSE : TRUE;
	SetScrollInfo(collectionsPanel.hWnd(), SB_VERT, &si, TRUE);
	ShowScrollBar(collectionsPanel.hWnd(), SB_VERT, scrollBarIsVisible);
	EnableScrollBar(collectionsPanel.hWnd(), SB_VERT, itemListHeight <= fHeight ? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
}