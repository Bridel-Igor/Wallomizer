#include "SetUserCollectionWindow.h"
#include "CollectionManager.h"
#include "MainWindow.h"
#include "Settings.h"

SetUserCollectionWindow *SetUserCollectionWindow::setUserCollectionWindow = nullptr;
UserCollection* SetUserCollectionWindow::currentUserCollection = nullptr;

LRESULT SetUserCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		stUsername = new Static			(Window(), "Username:",			10,		10,		80,		20, SS_RIGHT);
		edUsername = new Edit			(Window(), "",					100,	10,		150,	20);
		stCollectionID = new Static		(Window(), "Collection ID:",	10,		40,		80,		20, SS_RIGHT);
		cbCollections = new ComboBox	(Window(), "",					100,	40,		150,	20, 0, 0, nullptr);

		stPurity = new Static			(Window(), "Purity:",			10,		70,		80,		20, SS_RIGHT);
		btnPurSFW = new Button			(Window(), "SFW",				100,	70,		50,		20, BS_AUTOCHECKBOX | BS_PUSHLIKE);
		btnPurSketchy = new Button		(Window(), "Sketchy",			150,	70,		50,		20, BS_AUTOCHECKBOX | BS_PUSHLIKE);
		btnPurNSFW = new Button			(Window(), "NSFW",				200,	70,		50,		20, BS_AUTOCHECKBOX | BS_PUSHLIKE);

		btnCancel = new Button			(Window(), "Cancel",			10,		100,	80,		20);
		btnOk = new Button				(Window(), "Ok",				100,	100,	150,	20);

		edUsername->setTextA(currentUserCollection->settings->username);
		SendMessageA(btnPurSFW->hWnd, BM_SETCHECK, (WPARAM)(currentUserCollection->settings->categoriesAndPurity & S_PURITY_SFW), NULL);
		SendMessageA(btnPurSketchy->hWnd, BM_SETCHECK, (WPARAM)(currentUserCollection->settings->categoriesAndPurity & S_PURITY_SKETCHY), NULL);
		SendMessageA(btnPurNSFW->hWnd, BM_SETCHECK, (WPARAM)(currentUserCollection->settings->categoriesAndPurity & S_PURITY_NSFW), NULL);

		if (edUsername->isEmpty())
		{
			edUsername->setTextA(Settings::username);
			strcpy_s(currentUserCollection->settings->username, Settings::username);
		}

		if (currentUserCollection->settings->collectionID!=0 && strlen(currentUserCollection->settings->collectionName)!=0)
		{
			list.clear();
			UserCollection::UserCollectionInfo info;
			info.id = atoi(currentUserCollection->settings->collectionID);
			strcpy_s(info.label, currentUserCollection->settings->collectionName);
			list.push_back(info);
			SendMessageA(cbCollections->hWnd, CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd, CB_ADDSTRING, NULL, (LPARAM)list[0].label);
			SendMessageA(cbCollections->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = true;
		}

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		currentUserCollection = nullptr;
		delete stUsername, stCollectionID, stPurity;
		delete edUsername;
		delete cbCollections;
		delete btnOk, btnCancel, btnPurSFW, btnPurSketchy, btnPurNSFW;
		DeleteObject(font);
		DeleteObject(bkBrush);
		EnableWindow(MainWindow::mainWindow->Window(), TRUE);
		SetForegroundWindow(MainWindow::mainWindow->Window());
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
		if ((HWND)lParam==edUsername->hWnd && HIWORD(wParam) == EN_CHANGE)
		{
			SendMessageA(cbCollections->hWnd, CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd, CB_ADDSTRING, NULL, (LPARAM)"Click to update");
			SendMessageA(cbCollections->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = false;
			return 0;
		}
		
		if ((HWND)lParam == cbCollections->hWnd && HIWORD(wParam) == CBN_DROPDOWN)
		{
			char prevName[64] = {0};
			if (!list.empty() && validCollection)
				strcpy_s(prevName, list[cbCollections->getSelectedItem()].label);

			validCollection = false;
			SendMessageA(cbCollections->hWnd, CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd, CB_ADDSTRING, NULL, (LPARAM)"Updating...");
			SendMessageA(cbCollections->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			char tmpUsername[64];
			edUsername->getTextA(tmpUsername, 64);
			list.clear();
			list = UserCollection::loadCollectionList(tmpUsername, Settings::apiKey);
			SendMessageA(cbCollections->hWnd, CB_RESETCONTENT, NULL, NULL);
			if (list.empty())
			{
				SendMessageA(cbCollections->hWnd, CB_ADDSTRING, NULL, (LPARAM)"Empty");
				SendMessageA(cbCollections->hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				validCollection = false;
				return 0;
			}
			for (int i=0;i<list.size(); i++)
				SendMessageA(cbCollections->hWnd, CB_ADDSTRING, NULL, (LPARAM)list[i].label);

			int index = 0;
			if (strlen(prevName))
			{
				index = SendMessageA(cbCollections->hWnd, CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)prevName);
				if (index == CB_ERR)
					index = 0;
			}
			SendMessageA(cbCollections->hWnd, CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
			validCollection = true;
			return 0;
		}

		if COMMANDEVENT(btnOk)
		{
			if (edUsername->isEmpty() || !validCollection)
			{
				MessageBoxA(nullptr, "Invalid data", "wallhaven", MB_OK);
				return 0;
			}
			strcpy_s(currentUserCollection->settings->collectionName, list[cbCollections->getSelectedItem()].label);
			char cid[16] = { 0 };
			_itoa_s(list[cbCollections->getSelectedItem()].id, cid, 10);
			strcpy_s(currentUserCollection->settings->collectionID, cid);

			edUsername->getTextA(currentUserCollection->settings->username, 64);
			currentUserCollection->settings->categoriesAndPurity =
				S_PURITY_SFW * SendMessageA(btnPurSFW->hWnd, BM_GETCHECK, NULL, NULL) |
				S_PURITY_SKETCHY * SendMessageA(btnPurSketchy->hWnd, BM_GETCHECK, NULL, NULL) |
				S_PURITY_NSFW * SendMessageA(btnPurNSFW->hWnd, BM_GETCHECK, NULL, NULL);
			DestroyWindow(hWnd);
			CollectionManager::reloadSettings();
			return 0;
		}
		
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
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

void SetUserCollectionWindow::windowThread(UserCollection* collection)
{
	if (collection == nullptr)
		return;
	if (setUserCollectionWindow)
	{
		SetForegroundWindow(setUserCollectionWindow->Window());
		return;
	}
	currentUserCollection = collection;
	setUserCollectionWindow = new SetUserCollectionWindow;
	setUserCollectionWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 260, 130, NULL, NULL);
	ShowWindow(setUserCollectionWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setUserCollectionWindow->Window(), SW_HIDE);
	setUserCollectionWindow->Destroy();
	delete setUserCollectionWindow;
	setUserCollectionWindow = nullptr;
}