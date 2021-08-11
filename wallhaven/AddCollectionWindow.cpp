#include "AddCollectionWindow.h"
#include "CollectionManager.h"
#include "MainWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"

AddCollectionWindow* AddCollectionWindow::addCollectionWindow = nullptr;

LRESULT AddCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		btnAddUserCollection = new Button(Window(), "Add user collection",				10, 10, 180, 20);
		btnAddLocalCollection = new Button(Window(), "Add local collection",			10, 40, 180, 20);
		btnAddSearchCollection = new Button(Window(), "Add search collection",			10, 70, 180, 20);
		btnCancel = new Button(Window(), "Cancel",										10, 110, 180, 20);

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnAddUserCollection, btnAddLocalCollection, btnAddSearchCollection, btnCancel;
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
		if COMMANDEVENT(btnAddUserCollection)
		{
			ShowWindow(Window(), SW_HIDE);
			CollectionManager::addCollection<UserCollection>();
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnAddLocalCollection)
		{
			ShowWindow(Window(), SW_HIDE);
			CollectionManager::addCollection<LocalCollection>();
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnAddSearchCollection)
		{
			ShowWindow(Window(), SW_HIDE);
			CollectionManager::addCollection<SearchCollection>();
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(Window());
			return 0;
		}
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

void AddCollectionWindow::windowThread()
{
	if (addCollectionWindow)
	{
		SetForegroundWindow(addCollectionWindow->Window());
		return;
	}
	addCollectionWindow = new AddCollectionWindow;
	addCollectionWindow->Create("Add collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 200, 140, NULL, NULL);
	addCollectionWindow->centerWindow(MainWindow::mainWindow->Window());
	ShowWindow(addCollectionWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(addCollectionWindow->Window(), SW_HIDE);
	addCollectionWindow->Destroy();
	delete addCollectionWindow;
	addCollectionWindow = nullptr;
}