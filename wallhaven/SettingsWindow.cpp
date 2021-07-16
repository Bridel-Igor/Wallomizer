#include "SettingsWindow.h"
#include <CommCtrl.h>
#include "Settings.h"
#include "CollectionManager.h"
#include "AddCollectionWindow.h"

SettingsWindow* SettingsWindow::settingsWindow = nullptr;

void SettingsWindow::updateCollectionItems()
{	
	for (size_t i = CollectionManager::collections.size(); i < collectionItems.size(); i++)		//deleting excess items
	{
		delete collectionItems.back();
		collectionItems.pop_back();
	}

	for (size_t i = 0; i < collectionItems.size(); i++) // updating those which won't be created
		collectionItems[i]->updateInfo(CollectionManager::collections[i]);

	for (size_t i = collectionItems.size(); i < CollectionManager::collections.size(); i++) // creation
		if (CollectionManager::collections[i]!=nullptr)
			collectionItems.push_back(new CollectionItem(SettingsWindow::settingsWindow->Window(), 10, 40 + (i * 20), 620, 20, CollectionManager::collections[i], font));
}

void SettingsWindow::destroyCollectionItems()
{
	for (auto p : collectionItems)
		delete p;
	collectionItems.clear();
}

LRESULT SettingsWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stCollections = new Static(Window(), "Collections:", 20,	10,		100,	20);
		btnAdd = new Button(Window(), "Add..",				580,	10,		50,		20);
		
		updateCollectionItems();

		stDelay = new Static(Window(), "Delay:",			10,		450,	50,		20);
		stHours = new Static(Window(), "Hours",				60,		430,	60,		20, SS_CENTER);
		stMinutes = new Static(Window(), "Minutes",			130,	430,	60,		20, SS_CENTER);
		stSeconds = new Static(Window(), "Seconds",			200,	430,	60,		20, SS_CENTER);
		udeHours = new UpDownEdit(Window(),					60,		450,	60,		20, 0, 999, int((Settings::delay / 1000) / 3600));
		udeMinutes = new UpDownEdit(Window(),				130,	450,	60,		20, 0, 59,  int((Settings::delay / 1000) / 60) % 60);
		udeSeconds = new UpDownEdit(Window(),				200,	450,	60,		20, 0, 59,  int (Settings::delay / 1000) % 60);
		btnOk = new Button(Window(), "Set",					270,	450,	80,		20);

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		destroyCollectionItems();
		delete btnOk, btnAdd;
		delete stCollections, stHours, stMinutes, stSeconds, stDelay;
		delete udeHours, udeMinutes, udeSeconds;
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
		if (btnOk!=nullptr && (HMENU)wParam == btnOk->hMenu)
		{
			Settings::delay = (udeSeconds->getPos() + (udeMinutes->getPos()*60) + (udeHours->getPos()*3600)) * 1000;
			Settings::saveSettings();
			return 0;
		}
		if (btnAdd != nullptr && (HMENU)wParam == btnAdd->hMenu)
		{
			AddCollectionWindow::windowThread();
			return 0;
		}
		for (unsigned int i = 0; i < collectionItems.size(); i++)
		{
			if ((HMENU)wParam == collectionItems[i]->btnSettings->hMenu)
			{
				CollectionManager::collections[i]->openCollectionSettingsWindow();
				return 0;
			}
			if ((HMENU)wParam == collectionItems[i]->btnDelete->hMenu)
			{
				CollectionManager::eraseCollection(i);
				return 0;
			}
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		for (auto p : collectionItems)
			if (hWndStatic == p->stName->hWnd || hWndStatic == p->stNumber->hWnd)
			{
				SetTextColor(hdcStatic, CollectionItem::fontColor);
				SetBkColor(hdcStatic, CollectionItem::bkColor);
				return (LRESULT)CreateSolidBrush(CollectionItem::bkColor);
			}
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

void SettingsWindow::windowThread()
{
	if (settingsWindow)
	{
		SetForegroundWindow(settingsWindow->Window());
		return;
	}
	settingsWindow = new SettingsWindow;
	settingsWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 640, 480, NULL, NULL);
	ShowWindow(settingsWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(settingsWindow->Window(), SW_HIDE);
	settingsWindow->Destroy();
	delete settingsWindow;
	settingsWindow = nullptr;
}