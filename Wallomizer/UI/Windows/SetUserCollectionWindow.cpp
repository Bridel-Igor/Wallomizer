#include "SetUserCollectionWindow.h"
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
		EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);

		stUsername = new Static			(this->hWnd(), "Username:",			10,		10,		80,		20, SS_RIGHT);
		edUsername = new Edit			(this->hWnd(), "",					100,	10,		240,	20);
		stCollectionID = new Static		(this->hWnd(), "Collection ID:",	10,		40,		80,		20, SS_RIGHT);
		cbCollections = new ComboBox	(this->hWnd(), "",					100,	40,		240,	20);

		stPurity = new Static			(this->hWnd(), "Purity:",			10,		70,		80,		20, SS_RIGHT);
		purCom = new PurityComponent	(this->hWnd(),						100,	70,		240,	20);

		btnCancel = new Button			(this->hWnd(), "Cancel",			10,		100,	80,		20);
		btnOk = new Button				(this->hWnd(), "Ok",				100,	100,	240,	20);

		edUsername->setTextA(currentUserCollection->settings.sUsername);
		purCom->setPurity(currentUserCollection->settings.categoriesAndPurity);

		if (edUsername->isEmpty())
		{
			edUsername->setTextA(Settings::username);
			strcpy_s(currentUserCollection->settings.sUsername, Settings::username);
		}

		if (currentUserCollection->settings.sCollectionID!=0 && strlen(currentUserCollection->settings.sCollectionName)!=0)
		{
			list.clear();
			UserCollection::UserCollectionInfo info;
			info.id = atoi(currentUserCollection->settings.sCollectionID);
			strcpy_s(info.sLabel, currentUserCollection->settings.sCollectionName);
			list.push_back(info);
			SendMessageA(cbCollections->hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd(), CB_ADDSTRING, NULL, (LPARAM)list[0].sLabel);
			SendMessageA(cbCollections->hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = true;
		}

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		currentUserCollection = nullptr;

		delete stUsername;
		delete stCollectionID;
		delete stPurity;

		delete edUsername;
		delete cbCollections;

		delete btnOk;
		delete btnCancel;

		delete purCom;
		EnableWindow(MainWindow::mainWindow->hWnd(), TRUE);
		SetForegroundWindow(MainWindow::mainWindow->hWnd());
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
		if ((HWND)lParam==edUsername->hWnd() && HIWORD(wParam) == EN_CHANGE)
		{
			SendMessageA(cbCollections->hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Click to update");
			SendMessageA(cbCollections->hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = false;
			return 0;
		}
		
		if ((HWND)lParam == cbCollections->hWnd() && HIWORD(wParam) == CBN_DROPDOWN)
		{
			char prevName[64] = {0};
			if (!list.empty() && validCollection)
				strcpy_s(prevName, list[cbCollections->getSelectedItem()].sLabel);

			validCollection = false;
			SendMessageA(cbCollections->hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections->hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Updating...");
			SendMessageA(cbCollections->hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			char tmpUsername[64];
			edUsername->getTextA(tmpUsername, 64);
			list.clear();
			list = UserCollection::loadCollectionList(tmpUsername, Settings::apiKey);
			SendMessageA(cbCollections->hWnd(), CB_RESETCONTENT, NULL, NULL);
			if (list.empty())
			{
				SendMessageA(cbCollections->hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Empty");
				SendMessageA(cbCollections->hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				validCollection = false;
				return 0;
			}
			for (size_t i=0;i<list.size(); i++)
				SendMessageA(cbCollections->hWnd(), CB_ADDSTRING, NULL, (LPARAM)list[i].sLabel);

			int index = 0;
			if (strlen(prevName))
			{
				index = (int)SendMessageA(cbCollections->hWnd(), CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)prevName);
				if (index == CB_ERR)
					index = 0;
			}
			SendMessageA(cbCollections->hWnd(), CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
			validCollection = true;
			return 0;
		}

		if (btnOk->isClicked(wParam))
		{
			if (edUsername->isEmpty() || !validCollection)
			{
				MessageBoxA(nullptr, "Invalid data", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			strcpy_s(currentUserCollection->settings.sCollectionName, list[cbCollections->getSelectedItem()].sLabel);
			char cid[16] = { 0 };
			_itoa_s(list[cbCollections->getSelectedItem()].id, cid, 10);
			strcpy_s(currentUserCollection->settings.sCollectionID, cid);

			edUsername->getTextA(currentUserCollection->settings.sUsername, 64);
			currentUserCollection->settings.categoriesAndPurity = purCom->getPurity();
			if (currentUserCollection->isValid() == false)
				currentUserCollection->setValid(true);
			else
				collectionManager->reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}
		
		if (btnCancel->isClicked(wParam))
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		purCom->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void SetUserCollectionWindow::windowThread(UserCollection* collection, CollectionManager* _collectionManager)
{
	if (collection == nullptr)
		return;
	if (setUserCollectionWindow)
	{
		SetForegroundWindow(setUserCollectionWindow->hWnd());
		return;
	}
	currentUserCollection = collection;
	setUserCollectionWindow = new SetUserCollectionWindow;
	setUserCollectionWindow->collectionManager = _collectionManager;
	setUserCollectionWindow->Create("User collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL);
	setUserCollectionWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(setUserCollectionWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setUserCollectionWindow->hWnd(), SW_HIDE);
	setUserCollectionWindow->Destroy();
	delete setUserCollectionWindow;
	setUserCollectionWindow = nullptr;
}