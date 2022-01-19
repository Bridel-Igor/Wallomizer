#include <thread>

#include "MainWindow.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "AddCollectionWindow.h"
#include "SettingsWindow.h"
#include "TrayWindow.h"
#include "Delay.h"

MainWindow* MainWindow::mainWindow = nullptr;
MainWindow::CollectionItemsFrame* MainWindow::collectionItemsFrame = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////CollectionItemsFrame
void MainWindow::CollectionItemsFrame::updateCollectionItems()
{
	ShowWindow(stEmpty->hWnd, SW_HIDE);

	for (size_t i = CollectionManager::collections.size(); i < collectionItems.size(); i++) //deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(CollectionManager::collections[i]);

	for (size_t i = collectionItems.size(); i < CollectionManager::collections.size(); i++) // creation
		if (CollectionManager::collections[i]!=nullptr)
			collectionItems.push_back(new CollectionItem(MainWindow::collectionItemsFrame->Window(), 0, (int)(i * 27), MainWindow::width-20, 26, CollectionManager::collections[i], WindowStyles::mainFont));
	
	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->reposition(yCurrentScroll, scrollBarIsVisible);

	if (collectionItems.size()==0)
		ShowWindow(stEmpty->hWnd, SW_SHOW);

	InvalidateRect(collectionItemsFrame->Window(), NULL, FALSE);
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
	int itemListHeight = (int)collectionItems.size() * 27;
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
	SetScrollInfo(Window(), SB_VERT, &si, TRUE);
	ShowScrollBar(Window(), SB_VERT, scrollBarIsVisible);
	EnableScrollBar(Window(), SB_VERT, itemListHeight <= height? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH);
}

LRESULT MainWindow::CollectionItemsFrame::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stEmpty = new Static(Window(), "Collection list is empty. Click \"Add collection..\" button to add one.", 5, 0, 480, 20);
		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);

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
			if COMMANDEVENT(collectionItems[i]->btnSettings)
			{
				CollectionManager::collections[i]->openCollectionSettingsWindow();
				return 0;
			}
			if COMMANDEVENT(collectionItems[i]->btnDelete)
			{
				CollectionManager::eraseCollection(i);
				updateCollectionItems();
				InvalidateRect(Window(), nullptr, TRUE);
				return 0;
			}
			if COMMANDEVENT(collectionItems[i]->chboEnabled)
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					collectionItems[i]->chboEnabled->click();
					CollectionManager::collections[i]->isEnabled = collectionItems[i]->chboEnabled->isChecked();
					CollectionManager::reloadSettings();
					return 0;
				}
			}
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		for (int i = 0; i < collectionItems.size(); i++)
			collectionItems[i]->notify((HWND)wParam);
		return FALSE;
	}
	return 0;

	case WM_MOUSEWHEEL:
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam)>0)
			SendMessage(Window(), WM_VSCROLL, SB_LINEUP, 0L);
		else
			SendMessage(Window(), WM_VSCROLL, SB_LINEDOWN, 0L);
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

		ScrollWindowEx(Window(), 0, -yDelta, (CONST RECT*) NULL, (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL, SW_INVALIDATE);
		UpdateWindow(Window());
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		if ((HWND)lParam == stEmpty->hWnd)
		{
			HWND hWndStatic = (HWND)lParam;
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT)WindowStyles::collFrameBkBrush;
		}
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, WindowStyles::collItemFontColor);
		SetBkColor(hdcStatic, WindowStyles::collItemBkColor);
		return (LRESULT)WindowStyles::collItemBkBrush;
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////MainWindow
LRESULT MainWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stCollections = new Static(Window(), "Collections:",20,		10,		100,	20);
		btnAdd = new Button(Window(), "Add collection..",	530,	10,		100,	20);

		btnSettings = new Button(Window(), "Settings",		10,		450,	95,		20);
		player = new Player(Window(),						250,	450,					
															400,	450,	100,	20);	
		btnDonate = new Button(Window(), "Donate",			535,	450,	95,		20);
		
		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
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
		delete btnAdd;
		delete btnSettings;
		delete btnDonate;
		delete stCollections;
		delete player;
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
		if COMMANDEVENT(btnAdd)
		{
			AddCollectionWindow::windowThread();
			return 0;
		}
		if (player->click(wParam))
			return 0;
		if COMMANDEVENT(btnSettings)
		{
			SettingsWindow::windowThread();
			Settings::saveSettings();
			return 0;
		}
		if COMMANDEVENT(btnDonate)
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

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void MainWindow::windowThread()
{
	if (mainWindow)
	{
		SetForegroundWindow(mainWindow->Window());
		return;
	}
	while (!CollectionManager::isReady())
		Sleep(50);
	mainWindow = new MainWindow;
	collectionItemsFrame = new CollectionItemsFrame;
	mainWindow->Create("Wallomizer", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL);
	mainWindow->centerWindow(GetDesktopWindow());
	collectionItemsFrame->Create("", WS_CHILD | WS_BORDER | WS_VSCROLL, NULL, 10, 40, width-20, CollectionItemsFrame::height, mainWindow->Window(), NULL, false);
	ShowWindow(mainWindow->Window(), SW_SHOWNORMAL);
	ShowWindow(collectionItemsFrame->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(mainWindow->Window(), SW_HIDE);
	mainWindow->Destroy();
	delete mainWindow;
	mainWindow = nullptr;
	collectionItemsFrame->Destroy();
	delete collectionItemsFrame;
	collectionItemsFrame = nullptr;
}