#include "SetDirectoryCollectionWindow.h"
#include <ShObjIdl.h>
#include "CollectionManager.h"
#include "SettingsWindow.h"

SetDirectoryCollectionWindow* SetDirectoryCollectionWindow::setDirectoryCollectionWindow = nullptr;
DirectoryCollection* SetDirectoryCollectionWindow::currentDirectoryCollection = nullptr;

LRESULT SetDirectoryCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(SettingsWindow::settingsWindow->Window(), FALSE);

		stPath = new Static(Window(), "Enter path to directory:",	10,		10,		390,	20);
		edPath = new Edit(Window(), currentDirectoryCollection?currentDirectoryCollection->directoryPath:"",								
																	10,		30,		360,	20);
		btnPath = new Button(Window(), "..",						370,	30,		20,		20);
		btnCancel = new Button(Window(), "Cancel",					10,		60,		185,	20);
		btnOk = new Button(Window(), "Ok",							205,	60,		185,	20);
		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		currentDirectoryCollection = nullptr;
		delete stPath;
		delete edPath;
		delete btnOk, btnCancel, btnPath;
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
		if (btnOk!=nullptr && (HMENU)wParam == btnOk->hMenu)
		{
			if (!strlen(edPath->getTextA()))
			{
				MessageBoxA(nullptr, "Invalid data", "wallhaven", MB_OK);
				return 0;
			}
			strcpy_s(currentDirectoryCollection->directoryPath, edPath->getTextA());
			DestroyWindow(hWnd);
			CollectionManager::reloadSettings();
			return 0;
		}

		if (btnCancel != nullptr && (HMENU)wParam == btnCancel->hMenu)
		{
			DestroyWindow(hWnd);
			return 0;
		}

		if (btnPath != nullptr && (HMENU)wParam == btnPath->hMenu)
		{
			IFileDialog* pfd;
			LPWSTR g_path;
			if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
			{
				DWORD dwOptions;
				if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
				{
					pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
				}
				if (SUCCEEDED(pfd->Show(NULL)))
				{
					IShellItem* psi;
					if (SUCCEEDED(pfd->GetResult(&psi)))
					{
						if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &g_path)))
						{
							MessageBox(NULL, "GetIDListName() failed", NULL, NULL);
						}
						else
							edPath->setTextW(g_path);
						psi->Release();
					}
				}
				pfd->Release();
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

void SetDirectoryCollectionWindow::windowThread(DirectoryCollection *collection)
{
	if (setDirectoryCollectionWindow)
	{
		SetForegroundWindow(setDirectoryCollectionWindow->Window());
		return;
	}
	currentDirectoryCollection = collection;
	setDirectoryCollectionWindow = new SetDirectoryCollectionWindow;
	setDirectoryCollectionWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 400, 90, NULL, NULL);
	ShowWindow(setDirectoryCollectionWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setDirectoryCollectionWindow->Window(), SW_HIDE);
	setDirectoryCollectionWindow->Destroy();
	delete setDirectoryCollectionWindow;
	setDirectoryCollectionWindow = nullptr;
}