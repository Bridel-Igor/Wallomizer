#include <ShObjIdl.h>

#include "SetLocalCollectionWindow.h"
#include "MainWindow.h"

SetLocalCollectionWindow* SetLocalCollectionWindow::setLocalCollectionWindow = nullptr;
LocalCollection* SetLocalCollectionWindow::currentLocalCollection = nullptr;

LRESULT SetLocalCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);

		stPath = new Static(this->hWnd(), "Enter path to directory:",	10,		10,		390,	20);
		edPath = new Edit(this->hWnd(), currentLocalCollection?currentLocalCollection->directoryPath:L"",
																	10,		30,		360,	20);
		btnPath = new Button(this->hWnd(), "..",						370,	30,		20,		20);
		btnCancel = new Button(this->hWnd(), "Cancel",					10,		60,		185,	20);
		btnOk = new Button(this->hWnd(), "Ok",							205,	60,		185,	20);
		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		currentLocalCollection = nullptr;
		delete stPath;
		delete edPath;

		delete btnOk;
		delete btnCancel;
		delete btnPath;
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
		if (btnOk->isClicked(wParam))
		{
			if (edPath->isEmpty())
			{
				MessageBoxA(nullptr, "Path can't be empty.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			edPath->getTextW(currentLocalCollection->directoryPath, 255);
			if (currentLocalCollection->isValid == false)
				currentLocalCollection->isValid = true;
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

		if (btnPath->isClicked(wParam))
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

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void SetLocalCollectionWindow::windowThread(LocalCollection *collection, CollectionManager* _collectionManager)
{
	if (setLocalCollectionWindow)
	{
		SetForegroundWindow(setLocalCollectionWindow->hWnd());
		return;
	}
	currentLocalCollection = collection;
	setLocalCollectionWindow = new SetLocalCollectionWindow;
	setLocalCollectionWindow->collectionManager = _collectionManager;
	setLocalCollectionWindow->Create("Local collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 400, 90, NULL, NULL);
	setLocalCollectionWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(setLocalCollectionWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setLocalCollectionWindow->hWnd(), SW_HIDE);
	setLocalCollectionWindow->Destroy();
	delete setLocalCollectionWindow;
	setLocalCollectionWindow = nullptr;
}