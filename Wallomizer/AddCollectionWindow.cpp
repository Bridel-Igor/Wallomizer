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

		btnAddLocalCollection = new Button(Window(), "Add local collection",			10, 10, 220, 20);
		btnAddUserCollection = new Button(Window(), "Add wallhaven user collection",	10, 40, 220, 20);
		btnAddSearchCollection = new Button(Window(), "Add wallhaven search collection",10, 70, 220, 20);
		btnCancel = new Button(Window(), "Cancel",										10, 110, 220, 20);

		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnAddUserCollection;
		delete btnAddLocalCollection;
		delete btnAddSearchCollection;
		delete btnCancel;
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
		FillRect(hdc, &ps.rcPaint, WindowStyles::mainBkBrush);
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
	addCollectionWindow->Create("Add collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 240, 140, NULL, NULL);
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