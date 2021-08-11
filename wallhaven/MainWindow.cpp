#include "MainWindow.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "AddCollectionWindow.h"
#include "SettingsWindow.h"

MainWindow* MainWindow::mainWindow = nullptr;
MainWindow::CollectionItemsFrame* MainWindow::collectionItemsFrame = nullptr;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////CollectionItemsFrame
void MainWindow::CollectionItemsFrame::updateCollectionItems()
{
	for (size_t i = CollectionManager::collections.size(); i < collectionItems.size(); i++) //deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(CollectionManager::collections[i]);

	for (size_t i = collectionItems.size(); i < CollectionManager::collections.size(); i++) // creation
		if (CollectionManager::collections[i]!=nullptr)
			collectionItems.push_back(new CollectionItem(MainWindow::collectionItemsFrame->Window(), 0, (i * 20), MainWindow::width-20-18, 20, CollectionManager::collections[i], font));
	
	updateScroll();
	for (auto p : collectionItems) // placing according to the scrollbar
		p->scroll(yCurrentScroll);
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
		yMinScroll = 0;
		yCurrentScroll = 0;
		yMaxScroll = 0;
		bkBrush = CreateSolidBrush(RGB(15, 15, 15));
		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		updateCollectionItems();
	}
	return 0;

	case WM_DESTROY:
	{
		destroyCollectionItems();
		DeleteObject(font);
		DeleteObject(bkBrush);
	}
	return 0;

	case WM_PAINT:
	{
		hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, bkBrush);
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
				CollectionManager::saveSettings();
				CollectionManager::updateNumber();
				Settings::loadingImage.lock();
				remove("Resources/Loaded wallpaper.dat");
				Settings::loadingImage.unlock();
				Settings::abortDelay();
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
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, CollectionItem::fontColor);
		SetBkColor(hdcStatic, CollectionItem::bkColor);
		return (LRESULT)CreateSolidBrush(CollectionItem::bkColor);
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
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

		btnSetUserData = new Button(Window(), "Settings",	10,		450,	100,	20);

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnAdd, btnSetUserData;
		delete stCollections;
		DeleteObject(font);
		DeleteObject(bkBrush);
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, bkBrush);
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
		if COMMANDEVENT(btnSetUserData)
		{
			SettingsWindow::windowThread();
			Settings::saveSettings();
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(129, 193, 193));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)bkBrush;
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(200, 200, 200));
		SetDCBrushColor(hdc, RGB(200, 200, 200));
		return (LRESULT)GetStockObject(DC_BRUSH);
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
	mainWindow->Create("Wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL);
	mainWindow->centerWindow(GetDesktopWindow());
	collectionItemsFrame->Create("", WS_CHILD | WS_BORDER | WS_VSCROLL, NULL, 10, 40, width-20, CollectionItemsFrame::height, mainWindow->Window(), NULL);
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