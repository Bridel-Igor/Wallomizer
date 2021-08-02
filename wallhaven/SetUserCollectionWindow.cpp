#include "SetUserCollectionWindow.h"
#include "CollectionManager.h"
#include "SettingsWindow.h"

SetUserCollectionWindow *SetUserCollectionWindow::setUserCollectionWindow = nullptr;
UserCollection* SetUserCollectionWindow::currentUserCollection = nullptr;

LRESULT SetUserCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(SettingsWindow::settingsWindow->Window(), FALSE);

		stUsername = new Static			(Window(), "Username:",			10,		10,		100,	20, SS_RIGHT);
		edUsername = new Edit			(Window(), currentUserCollection ? currentUserCollection->settings->username : "",
																		120,	10,		110,	20);
		stCollectionID = new Static		(Window(), "Collection ID:",	10,		40,		100,	20, SS_RIGHT);
		edCollectionID = new Edit		(Window(), currentUserCollection ? currentUserCollection->settings->collectionID : "",
																		120,	40,		110,	20, ES_NUMBER);
		stIsApiKeyUsed = new Static		(Window(), "Use API key:",		10,		70,		100,	20, SS_RIGHT);
		cbIsApiKeyUsed = new CheckBox	(Window(), "",					120,	70,		20,		20, (HINSTANCE)GetWindowLongPtr(Window(), GWLP_HINSTANCE), currentUserCollection ? currentUserCollection->settings->isApiKeyUsed : true);
		stApiKey = new Static			(Window(), "Api key:",			10,		100,	100,	20, SS_RIGHT);
		edApiKey = new Edit				(Window(), currentUserCollection ? currentUserCollection->settings->apiKey : "",
																		120,	100,	110,	20);
		EnableWindow(edApiKey->hWnd, cbIsApiKeyUsed->isChecked());
		btnCancel = new Button			(Window(), "Cancel",			10,		130,	100,	20);
		btnOk = new Button				(Window(), "Ok",				120,	130,	110,	20);
		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		currentUserCollection = nullptr;
		delete stUsername, stCollectionID, stApiKey, stIsApiKeyUsed;
		delete edUsername, edCollectionID, edApiKey;
		delete cbIsApiKeyUsed;
		delete btnOk, btnCancel;
		DeleteObject(font);
		DeleteObject(bkBrush);
		EnableWindow(SettingsWindow::settingsWindow->Window(), TRUE);
		SetForegroundWindow(SettingsWindow::settingsWindow->Window());
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
		if COMMANDEVENT(btnOk)
		{
			if (!strlen(edUsername->getTextA()) || !strlen(edCollectionID->getTextA()) || (cbIsApiKeyUsed->isChecked() && !strlen(edApiKey->getTextA())))
			{
				MessageBoxA(nullptr, "Invalid data", "wallhaven", MB_OK);
				return 0;
			}
			strcpy_s(currentUserCollection->settings->username, edUsername->getTextA());
			strcpy_s(currentUserCollection->settings->collectionID, edCollectionID->getTextA());
			currentUserCollection->settings->isApiKeyUsed = cbIsApiKeyUsed->isChecked();
			if (cbIsApiKeyUsed->isChecked())
				strcpy_s(currentUserCollection->settings->apiKey, edApiKey->getTextA());
			DestroyWindow(hWnd);
			CollectionManager::reloadSettings();
			return 0;
		}
		
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(hWnd);
			return 0;
		}
		
		if COMMANDEVENT(cbIsApiKeyUsed)
		{
			EnableWindow(edApiKey->hWnd, cbIsApiKeyUsed->isChecked());
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
	if (setUserCollectionWindow)
	{
		SetForegroundWindow(setUserCollectionWindow->Window());
		return;
	}
	currentUserCollection = collection;
	setUserCollectionWindow = new SetUserCollectionWindow;
	setUserCollectionWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 240, 160, NULL, NULL);
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