#include <thread>

#include "MainWindow.h"
#include "Settings.h"
#include "AddCollectionWindow.h"
#include "SettingsWindow.h"
#include "TrayWindow.h"
#include "Delay.h"

MainWindow* MainWindow::mainWindow = nullptr;
MainWindow::CollectionItemsFrame* MainWindow::collectionItemsFrame = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////CollectionItemsFrame
void MainWindow::CollectionItemsFrame::updateCollectionItems()
{
	ShowWindow(stEmpty->hWnd(), SW_HIDE);

	for (size_t i = collectionManager->m_pCollections.size(); i < collectionItems.size(); i++) //deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(collectionManager->m_pCollections[i]);

	for (size_t i = collectionItems.size(); i < collectionManager->m_pCollections.size(); i++) // creation
		if (collectionManager->m_pCollections[i]!=nullptr)
			collectionItems.push_back(new CollectionItem(MainWindow::collectionItemsFrame->hWnd(), 0, (int)(i * (CollectionItem::height + 1)), MainWindow::width-20, collectionManager->m_pCollections[i], WindowStyles::mainFont));
	
	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->reposition(yCurrentScroll, scrollBarIsVisible);

	if (collectionItems.size()==0)
		ShowWindow(stEmpty->hWnd(), SW_SHOW);

	InvalidateRect(collectionItemsFrame->hWnd(), NULL, FALSE);
}

void MainWindow::CollectionItemsFrame::destroyCollectionItems()
{
	for (auto p : collectionItems)
		delete p;
	collectionItems.clear();
	updateScroll();
}

void MainWindow::CollectionItemsFrame::updateScroll()
{
	int itemListHeight = (int)collectionItems.size() * (CollectionItem::height + 1);
	yMaxScroll = max(itemListHeight - height, 0);
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	yCurrentScroll = yCurrentScroll < 0 ? 0 : yCurrentScroll;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = itemListHeight;
	si.nPage = height;
	si.nPos = yCurrentScroll;
	
	scrollBarIsVisible = itemListHeight <= height ? FALSE : TRUE;
	SetScrollInfo(hWnd(), SB_VERT, &si, TRUE);
	ShowScrollBar(hWnd(), SB_VERT, scrollBarIsVisible);
	EnableScrollBar(hWnd(), SB_VERT, itemListHeight <= height? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
}

LRESULT MainWindow::CollectionItemsFrame::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stEmpty = new Static(this->hWnd(), "Collection list is empty. Click \"Add collection..\" button to add one.", 5, 0, 480, 20);
		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);

		yMinScroll = 0;
		yCurrentScroll = 0;
		yMaxScroll = 0;
		updateCollectionItems();
	}
	return 0;

	case WM_DESTROY:
	{
		destroyCollectionItems();
		delete stEmpty;
	}
	return 0;

	case WM_PAINT:
	{
		hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::collFrameBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		for (int i=0;i<collectionItems.size();i++)
			if (collectionItems[i]->draw(pDIS))
				return TRUE;
	}
	return 0;

	case WM_COMMAND:
	{
		for (unsigned int i = 0; i < collectionItems.size(); i++)
		{
			if (collectionItems[i]->btnSettings->isClicked(wParam))
			{
				collectionManager->m_pCollections[i]->openCollectionSettingsWindow();
				return 0;
			}
			if (collectionItems[i]->btnDelete->isClicked(wParam))
			{
				collectionManager->eraseCollection(i);
				updateCollectionItems();
				InvalidateRect(this->hWnd(), nullptr, TRUE);
				return 0;
			}
			if (collectionItems[i]->chboEnabled->isClicked(wParam))
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					collectionItems[i]->chboEnabled->click();
					collectionManager->m_pCollections[i]->setEnabled(collectionItems[i]->chboEnabled->isChecked());
					collectionManager->reloadSettings();
					return 0;
				}
			}
		}
	}
	return 0;

	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			SendMessage(this->hWnd(), WM_VSCROLL, SB_LINEUP, 0L);
		else
			SendMessage(this->hWnd(), WM_VSCROLL, SB_LINEDOWN, 0L);
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

		ScrollWindowEx(this->hWnd(), 0, -yDelta, (CONST RECT*) NULL, (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL, SW_INVALIDATE);
		UpdateWindow(this->hWnd());
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;

		for (int i = 0; i < collectionItems.size(); i++)
			if (hWndStatic == collectionItems[i]->stNumber->hWnd())
			{
				if (collectionItems[i]->chboEnabled->isChecked())
					SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
				else
					SetTextColor(hdcStatic, RGB(80, 80, 80));
				SetBkColor(hdcStatic, WindowStyles::collItemBkColor);
				return (LRESULT)WindowStyles::collItemBkBrush;
			}

		if (hWndStatic == stEmpty->hWnd())
		{
			SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT)WindowStyles::collFrameBkBrush;
		}
		SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
		SetBkColor(hdcStatic, WindowStyles::collItemBkColor);
		return (LRESULT)WindowStyles::collItemBkBrush;
	}
	return 0;

	case WM_SETCURSOR:
	{
		for (int i = 0; i < collectionItems.size(); i++)
			collectionItems[i]->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////MainWindow
MainWindow::MainWindow(CollectionManager* pCollectionManager) :
	IWindow("Wallomizer", "Main Window Class",  WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_pCollectionManager(pCollectionManager)
{
	if (mainWindow)
	{
		SetForegroundWindow(mainWindow->hWnd());
		return;
	}
	mainWindow = this;
	while (!m_pCollectionManager->isReady())
		Sleep(50);
	mainWindow->centerWindow(GetDesktopWindow());

	stCollections = new Static(this->hWnd(), "Collections:",	20,		10,		100,	20);
	btnAdd = new Button(this->hWnd(), "Add collection..",		530,	10,		100,	20);

	btnSettings = new Button(this->hWnd(), "Settings",			10,		450,	95,		20);
	player = new Player(this->hWnd(),							250,	450,
																400,	450,	100,	20, m_pCollectionManager);
	btnDonate = new Button(this->hWnd(), "Donate",				535,	450,	95,		20);

	EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);

	collectionItemsFrame = new CollectionItemsFrame;
	collectionItemsFrame->collectionManager = m_pCollectionManager;
	collectionItemsFrame->Create("", WS_CHILD | WS_BORDER | WS_VSCROLL, NULL, 10, 40, width - 20, CollectionItemsFrame::height, mainWindow->hWnd(), NULL, false);
	
	player->updateTimer(true);

	ShowWindow(mainWindow->hWnd(), SW_SHOWNORMAL);
	ShowWindow(collectionItemsFrame->hWnd(), SW_SHOWNORMAL);
}

MainWindow::~MainWindow()
{
	mainWindow = nullptr;

	ShowWindow(hWnd(), SW_HIDE);

	delete btnAdd;
	delete btnSettings;
	delete btnDonate;
	delete stCollections;
	delete player;

	collectionItemsFrame->Destroy();
	delete collectionItemsFrame;
	collectionItemsFrame = nullptr;
}

LRESULT MainWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	return 0;

	case WM_SHOWWINDOW:
	{
		if (wParam == TRUE)
		{
			player->updateTimer(true);
			player->redrawPlayers();
		}
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player->draw(pDIS))
			return TRUE;
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::mainBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if (btnAdd->isClicked(wParam))
		{
			AddCollectionWindow addCollectionWindow(m_pCollectionManager);
			addCollectionWindow.windowLoop();
			return 0;
		}
		if (player->click(wParam))
			return 0;
		if (btnSettings->isClicked(wParam))
		{
			SettingsWindow::windowThread();
			Settings::saveSettings();
			return 0;
		}
		if (btnDonate->isClicked(wParam))
		{
			ShellExecute(0, 0, "https://donatello.to/IgorBridel", 0, 0, SW_SHOW);
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, WindowStyles::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)WindowStyles::mainBkBrush;
	}
	return 0;

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;
	
	case WM_SETCURSOR:
	{
		player->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	} 

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

