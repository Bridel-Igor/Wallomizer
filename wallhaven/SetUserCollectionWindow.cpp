#include "SetUserCollectionWindow.h"
#include "UserCollectionSettings.h"

#define BUTTON_OK		1
#define BUTTON_CANCEL	2
#define CHECKBOX_ISAPI	3

//bool SetUserCollectionWindow::running = false;
SetUserCollectionWindow *SetUserCollectionWindow::setUserCollectionWindow = nullptr;

BOOL CALLBACK SetChildFont(HWND hwndChild, LPARAM lParam)
{
	HFONT hFont = (HFONT)lParam;
	SendMessage(hwndChild, WM_SETFONT, (WPARAM)hFont, TRUE);
	return TRUE;
}

LRESULT SetUserCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		stDelay = new Static			(Window(), "Delay (seconds):",	10,		10,		100,	20, SS_RIGHT);
		edDelay = new Edit				(Window(), "",					120,	10,		110,	20, ES_NUMBER);
		stUsername = new Static			(Window(), "Username:",			10,		40,		100,	20, SS_RIGHT);
		edUsername = new Edit			(Window(), "",					120,	40,		110,	20);
		stCollectionID = new Static		(Window(), "Collection ID:",	10,		70,		100,	20, SS_RIGHT);
		edCollectionID = new Edit		(Window(), "",					120,	70,		110,	20, ES_NUMBER);
		stIsApiKeyUsed = new Static		(Window(), "Use API key:",		10,		100,	100,	20, SS_RIGHT);
		cbIsApiKeyUsed = new CheckBox	(Window(), "",					120,	100,	20,		20, (HMENU)CHECKBOX_ISAPI, (HINSTANCE)GetWindowLongPtr(Window(), GWLP_HINSTANCE), true);
		stApiKey = new Static			(Window(), "Api key:",			10,		130,	100,	20, SS_RIGHT);
		edApiKey = new Edit				(Window(), "",					120,	130,	110,	20);
		btnOk = new Button				(Window(), "Ok",				10,		160,	100,	20, (HMENU)BUTTON_OK);
		btnCancel = new Button			(Window(), "Cancel",			120,	160,	110,	20, (HMENU)BUTTON_CANCEL);
		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		delete stDelay, stUsername, stCollectionID, stApiKey, stIsApiKeyUsed;
		delete edDelay, edUsername, edCollectionID, edApiKey;
		delete cbIsApiKeyUsed;
		delete btnOk, btnCancel;
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
		switch (wParam)
		{
			case BUTTON_OK:
			{
				if (!strlen(edDelay->getText()) || !strlen(edUsername->getText()) || !strlen(edCollectionID->getText()) || (cbIsApiKeyUsed->isChecked() && !strlen(edApiKey->getText())))
				{
					MessageBoxA(nullptr, "Invalid data", "wallhaven", MB_OK);
					return 0;
				}
				UserCollectionSettings* settings = new UserCollectionSettings;
				settings->delay = atoi(edDelay->getText())*1000;
				strcpy_s(settings->username, edUsername->getText());
				strcpy_s(settings->collectionID, edCollectionID->getText());
				settings->isApiKeyUsed = cbIsApiKeyUsed->isChecked();
				if (cbIsApiKeyUsed->isChecked())
					strcpy_s(settings->apiKey, edApiKey->getText());
				settings->saveSettings();
				delete settings;
				SendMessageA(hWnd, WM_DESTROY, NULL, NULL);
			}
			return 0;

			case BUTTON_CANCEL:
			{
				exit(0);
			}
			return 0;

			case CHECKBOX_ISAPI:
			{
				if (cbIsApiKeyUsed->isChecked())
					EnableWindow(edApiKey->hWnd, TRUE);
				else
					EnableWindow(edApiKey->hWnd, FALSE);
			}
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

void SetUserCollectionWindow::windowThread()
{
	if (setUserCollectionWindow)
	{
		SetForegroundWindow(setUserCollectionWindow->Window());
		return;
	}
	setUserCollectionWindow = new SetUserCollectionWindow;
	setUserCollectionWindow->Create("wallhaven", WS_OVERLAPPED | WS_SYSMENU, NULL, 100, 100, 256, 229, NULL, NULL);
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

//#include <ShObjIdl.h>//
//void OpenDlg()
//{
	//HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
	//    COINIT_DISABLE_OLE1DDE);
	//if (SUCCEEDED(hr))
	//{
	//    IFileOpenDialog* pFileOpen;

	//    // Create the FileOpenDialog object.
	//    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
	//        IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	//    if (SUCCEEDED(hr))
	//    {
	//        // Show the Open dialog box.
	//        hr = pFileOpen->Show(NULL);

	//        // Get the file name from the dialog box.
	//        if (SUCCEEDED(hr))
	//        {
	//            IShellItem* pItem;
	//            hr = pFileOpen->GetResult(&pItem);
	//            if (SUCCEEDED(hr))
	//            {
	//                PWSTR pszFilePath;
	//                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	//                // Display the file name to the user.
	//                if (SUCCEEDED(hr))
	//                {
	//                    MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
	//                    CoTaskMemFree(pszFilePath);
	//                }
	//                pItem->Release();
	//            }
	//        }
	//        pFileOpen->Release();
	//    }
	//    CoUninitialize();
	//}
	//return 0;
//}