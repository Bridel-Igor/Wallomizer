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
			collectionItems.push_back(new CollectionItem(MainWindow::collectionItemsFrame->Window(), 0, (i * 20), MainWindow::width-20-18, 20, CollectionManager::collections[i], WindowStyles::mainFont));
	
	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->scroll(yCurrentScroll);

	if (collectionItems.size()==0)
		ShowWindow(stEmpty->hWnd, SW_SHOW);
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
	int itemListHeight = (int)collectionItems.size() * 20;
	yMaxScroll = max(itemListHeight - height, 0);
	yCurrentScroll = min(yCurrentScroll, yMaxScroll);
	yCurrentScroll = yCurrentScroll < 0 ? 0 : yCurrentScroll;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = yMinScroll;
	si.nMax = itemListHeight;
	si.nPage = height;
	si.nPos = yCurrentScroll;
	
	SetScrollInfo(Window(), SB_VERT, &si, TRUE);
	ShowScrollBar(Window(), SB_VERT, TRUE);
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
				CollectionManager::collections[i]->isEnabled = collectionItems[i]->chboEnabled->isChecked();
				CollectionManager::reloadSettings();
				return 0;
			}
		}
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

		updateCollectionItems();
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
void MainWindow::updateTimer()
{
	if (stDelayRemained == nullptr)
		return;
	unsigned long remaining = Delay::getRemainingDelay();
	char buf[16], sec[3], min[3], hour[4];
	_itoa_s((remaining / 1000) % 60, sec, 10);
	_itoa_s(((remaining / 1000) /60) %60, min, 10);
	_itoa_s((remaining / 1000) /3600, hour, 10);
	strcpy_s(buf, hour);
	strcat_s(buf, ":");
	strcat_s(buf, min);
	strcat_s(buf, ":");
	strcat_s(buf, sec);
	SetWindowTextA(stDelayRemained->hWnd, buf);
}

LRESULT MainWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stCollections = new Static(Window(), "Collections:",20,		10,		100,	20);
		btnAdd = new Button(Window(), "Add collection..",	530,	10,		100,	20);

		btnSettings = new Button(Window(), "Settings",		10,		450,	95,		20);

		btnPrev = new Button(Window(), "",					250,	450,	20,		20, BS_OWNERDRAW);
		btnOpenExternal = new Button(Window(), "",			280,	450,	20,		20, BS_OWNERDRAW);
		btnPlay = new Button(Window(), "",					310,	450,	20,		20, BS_OWNERDRAW);
		btnPause = new Button(Window(), "",					340,	450,	20,		20, BS_OWNERDRAW);
		btnNext = new Button(Window(), "",					370,	450,	20,		20, BS_OWNERDRAW);
		stDelayRemained = new Static(Window(), "",			400,	450,	100,	20);

		hIPlay = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PLAY), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIPlayActive = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PLAY_ACTIVE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIPause = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PAUSE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIPauseActive = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PAUSE_ACTIVE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIPrevEnabled = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PREV), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIPrevDisabled = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PREV_DISABLED), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hINextEnabled = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_NEXT), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		hIOpenExternal = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_OPEN_EXTERNAL), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		
		btnDonate = new Button(Window(), "Donate",			535,	450,	95,		20);
		
		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);

		updateTimer();
	}
	return 0;

	case WM_DESTROY:
	{
		DestroyIcon(hIPlay);
		DestroyIcon(hIPlayActive);
		DestroyIcon(hIPause);
		DestroyIcon(hIPauseActive);
		DestroyIcon(hIPrevEnabled);
		DestroyIcon(hIPrevDisabled);
		DestroyIcon(hINextEnabled);
		DestroyIcon(hIOpenExternal);
		delete btnAdd, btnSettings, btnPlay, btnPause, btnDonate, btnNext, btnPrev, btnOpenExternal;
		delete stCollections, stDelayRemained;
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
		if (pDIS->hwndItem == btnPlay->hWnd)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, Delay::bRunSlideshow?hIPlayActive:hIPlay, 0, 0, 0, NULL, DI_NORMAL);
			return TRUE;
		}
		if (pDIS->hwndItem == btnPause->hWnd)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, Delay::bRunSlideshow?hIPause:hIPauseActive, 0, 0, 0, NULL, DI_NORMAL);
			return TRUE;
		}
		if (pDIS->hwndItem == btnNext->hWnd)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, hINextEnabled, 0, 0, 0, NULL, DI_NORMAL);
			return TRUE;
		}
		if (pDIS->hwndItem == btnPrev->hWnd)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, CollectionManager::isPrevious() ? hIPrevEnabled : hIPrevDisabled, 0, 0, 0, NULL, DI_NORMAL);
			return TRUE;
		}
		if (pDIS->hwndItem == btnOpenExternal->hWnd)
		{
			FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
			DrawIconEx(pDIS->hDC, 0, 0, hIOpenExternal, 0, 0, 0, NULL, DI_NORMAL);
			return TRUE;
		}
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
		if COMMANDEVENT(btnOpenExternal)
		{
			CollectionManager::openWallpaperExternal();
			return 0;
		}
		if COMMANDEVENT(btnPrev)
		{
			PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLOMIZER_PREVIOUSWALLPAPER, NULL);
			return 0;
		}
		if COMMANDEVENT(btnPlay)
		{
			PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLOMIZER_START, NULL);
			return 0;
		}
		if COMMANDEVENT(btnPause)
		{
			PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLOMIZER_PAUSE, NULL);
			return 0;
		}
		if COMMANDEVENT(btnNext)
		{
			PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, ID_WALLOMIZER_NEXTWALLPAPER, NULL);
			return 0;
		}
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