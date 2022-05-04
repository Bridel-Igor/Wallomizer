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
	stCollections		(hWnd(), "Collections:",		20,		10,		100,	20),
	btnAdd				(hWnd(), "Add collection..",	530,	10,		100,	20),

	fX(10), fY(40), fWidth(width - 20), fHeight(400), bkColor(RGB(15, 15, 15)), bkBrush(CreateSolidBrush(bkColor)),
	collectionsPanel	(hWnd(), "CollectionPanelClass",fX,		fY,		fWidth, fHeight, bkBrush),
	stEmpty				(collectionsPanel.hWnd(), "Collection list is empty. Click \"Add collection..\" button to add one.", 5, 0, 480, 20),

	btnSettings			(hWnd(), "Settings",			10,		450,	95,		20),
	player				(hWnd(),						250,	450,
														400,	450,	100,	20, m_pCollectionManager),
	btnDonate			(hWnd(), "Donate",				535,	450,	95,		20)
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
		for (auto item : collectionItems)
			if (item->draw(pDIS))
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
		if (btnDonate.isClicked(wParam))
		{
			ShellExecute(0, 0, "https://donatello.to/IgorBridel", 0, 0, SW_SHOW);
			return 0;
		}
		for (int i=0; i<collectionItems.size(); i++)
		{
			if (collectionItems[i]->btnSettings.isClicked(wParam))
			{
				m_pCollectionManager->m_pCollections[i]->openCollectionSettingsWindow(hWnd());
				return 0;
			}
			if (collectionItems[i]->btnDelete.isClicked(wParam))
			{
				m_pCollectionManager->eraseCollection(i);
				updateCollectionItems();
				InvalidateRect(collectionsPanel.hWnd(), nullptr, TRUE);
				return 0;
			}
			if (collectionItems[i]->chboEnabled.isClicked(wParam))
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					collectionItems[i]->chboEnabled.click();
					m_pCollectionManager->m_pCollections[i]->setEnabled(collectionItems[i]->chboEnabled.isChecked());
					m_pCollectionManager->reloadSettings();
					return 0;
				}
			}
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
		for (auto p : collectionItems) // placing according to the scrollbar
			p->reposition(yCurrentScroll, scrollBarIsVisible);

		ScrollWindowEx(collectionsPanel.hWnd(), 0, -yDelta, nullptr, nullptr, (HRGN)NULL, (PRECT)NULL, SW_INVALIDATE);
		UpdateWindow(collectionsPanel.hWnd());
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		HWND hWnd = (HWND)lParam;
		HDC hdc = (HDC)wParam;
		for (auto item : collectionItems)
		{
			if (hWnd == item->stNumber.hWnd() || hWnd == item->stName.hWnd())
			{
				if (item->chboEnabled.isChecked())
					SetTextColor(hdc, CollectionItem::Resources::collItemFontColor);
				else
					SetTextColor(hdc, RGB(80, 80, 80));
				SetBkColor(hdc, CollectionItem::Resources::collItemBkColor);
				return (LRESULT)CollectionItem::Resources::collItemBkBrush;
			}
			if (hWnd == item->chboEnabled.hWnd() ||
				hWnd == item->btnDelete.hWnd() ||
				hWnd == item->btnSettings.hWnd())
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

	case WM_SETCURSOR:
	{
		player.mouseHovering(wParam);
		for (auto item : collectionItems)
			item->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	} 
	}
	return RESULT_DEFAULT;
}

void MainWindow::updateCollectionItems()
{
	ShowWindow(stEmpty.hWnd(), SW_HIDE);

	for (size_t i = m_pCollectionManager->m_pCollections.size(); i < collectionItems.size(); i++) //deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(m_pCollectionManager->m_pCollections[i]);

	for (size_t i = collectionItems.size(); i < m_pCollectionManager->m_pCollections.size(); i++) // creation
		if (m_pCollectionManager->m_pCollections[i] != nullptr)
			collectionItems.push_back(new CollectionItem(collectionsPanel.hWnd(), 0, (int)(i * (CollectionItem::height + 1)), fWidth, m_pCollectionManager->m_pCollections[i], IWindow::Resources::mainFont));

	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->reposition(yCurrentScroll, scrollBarIsVisible);

	if (collectionItems.size() == 0)
		ShowWindow(stEmpty.hWnd(), SW_SHOW);

	InvalidateRect(hWnd(), NULL, FALSE);
}

void MainWindow::destroyCollectionItems()
{
	for (auto p : collectionItems)
		delete p;
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