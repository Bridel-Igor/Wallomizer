#include "AddCollectionWindow.h"
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
		EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);

		btnAddLocalCollection = new Button(this->hWnd(), "Add local collection",			10, 10, 220, 20);
		btnAddUserCollection = new Button(this->hWnd(), "Add wallhaven user collection",	10, 40, 220, 20);
		btnAddSearchCollection = new Button(this->hWnd(), "Add wallhaven search collection",10, 70, 220, 20);
		btnCancel = new Button(this->hWnd(), "Cancel",										10, 110, 220, 20);

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnAddUserCollection;
		delete btnAddLocalCollection;
		delete btnAddSearchCollection;
		delete btnCancel;
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
		if (btnAddUserCollection->isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			collectionManager->addCollection<UserCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnAddLocalCollection->isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			collectionManager->addCollection<LocalCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnAddSearchCollection->isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			collectionManager->addCollection<SearchCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnCancel->isClicked(wParam))
		{
			DestroyWindow(this->hWnd());
			return 0;
		}
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void AddCollectionWindow::windowThread(CollectionManager* _collectionManager)
{
	if (addCollectionWindow)
	{
		SetForegroundWindow(addCollectionWindow->hWnd());
		return;
	}
	addCollectionWindow = new AddCollectionWindow;
	addCollectionWindow->collectionManager = _collectionManager;
	addCollectionWindow->Create("Add collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 240, 140, NULL, NULL);
	addCollectionWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(addCollectionWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(addCollectionWindow->hWnd(), SW_HIDE);
	addCollectionWindow->Destroy();
	delete addCollectionWindow;
	addCollectionWindow = nullptr;
}