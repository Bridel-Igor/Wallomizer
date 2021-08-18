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
		edUsername = new Edit			(Window(), "",					100,	10,		240,	20);
		stCollectionID = new Static		(Window(), "Collection ID:",	10,		40,		80,		20, SS_RIGHT);
		cbCollections = new ComboBox	(Window(), "",					100,	40,		240,	20);

		stPurity = new Static			(Window(), "Purity:",			10,		70,		80,		20, SS_RIGHT);
		purCom = new PurityComponent	(Window(),						100,	70,		240,	20);

		btnCancel = new Button			(Window(), "Cancel",			10,		100,	80,		20);
		btnOk = new Button				(Window(), "Ok",				100,	100,	240,	20);

		edUsername->setTextA(currentUserCollection->settings->username);
		purCom->setPurity(currentUserCollection->settings->categoriesAndPurity);

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

		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		currentUserCollection = nullptr;
		delete stUsername, stCollectionID, stPurity;
		delete edUsername;
		delete cbCollections;
		delete btnOk, btnCancel;
		delete purCom;
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

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (purCom->draw(pDIS))
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
		if (purCom->click(wParam))
			return 0;
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
			for (size_t i=0;i<list.size(); i++)
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
			currentUserCollection->settings->categoriesAndPurity = purCom->getPurity();
			if (currentUserCollection->isValid == false)
				currentUserCollection->isValid = true;
			else
				CollectionManager::reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}
		
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(hWnd);
			return 0;
		}
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
	setUserCollectionWindow->Create("User collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL);
	setUserCollectionWindow->centerWindow(MainWindow::mainWindow->Window());
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