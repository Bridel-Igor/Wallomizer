#include <ShObjIdl.h>

#include "SetLocalCollectionWindow.h"
#include "CollectionManager.h"
#include "MainWindow.h"

SetLocalCollectionWindow* SetLocalCollectionWindow::setLocalCollectionWindow = nullptr;
LocalCollection* SetLocalCollectionWindow::currentLocalCollection = nullptr;

LRESULT SetLocalCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		stPath = new Static(Window(), "Enter path to directory:",	10,		10,		390,	20);
		edPath = new Edit(Window(), currentLocalCollection?currentLocalCollection->directoryPath:"",								
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
		currentLocalCollection = nullptr;
		delete stPath;
		delete edPath;
		delete btnOk, btnCancel, btnPath;
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
		if COMMANDEVENT(btnOk)
		{
			if (edPath->isEmpty())
			{
				MessageBoxA(nullptr, "Invalid data", "wallhaven", MB_OK);
				return 0;
			}
			edPath->getTextA(currentLocalCollection->directoryPath, 255);
			DestroyWindow(hWnd);
			CollectionManager::reloadSettings();
			return 0;
		}

		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(hWnd);
			return 0;
		}

		if COMMANDEVENT(btnPath)
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

void SetLocalCollectionWindow::windowThread(LocalCollection *collection)
{
	if (setLocalCollectionWindow)
	{
		SetForegroundWindow(setLocalCollectionWindow->Window());
		return;
	}
	currentLocalCollection = collection;
	setLocalCollectionWindow = new SetLocalCollectionWindow;
	setLocalCollectionWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 400, 90, NULL, NULL);
	ShowWindow(setLocalCollectionWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setLocalCollectionWindow->Window(), SW_HIDE);
	setLocalCollectionWindow->Destroy();
	delete setLocalCollectionWindow;
	setLocalCollectionWindow = nullptr;
}