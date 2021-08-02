#include "AddCollectionWindow.h"
#include "CollectionManager.h"
#include "SettingsWindow.h"
#include "UserCollection.h"
#include "DirectoryCollection.h"

AddCollectionWindow* AddCollectionWindow::addCollectionWindow = nullptr;

LRESULT AddCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(SettingsWindow::settingsWindow->Window(), FALSE);

		btnAddUserCollection = new Button(Window(), "Add user collection",				10, 10, 180, 20);
		btnAddDirectoryCollection = new Button(Window(), "Add dirrectory collection",	10, 40, 180, 20);
		btnCancel = new Button(Window(), "Cancel",										10, 80, 180, 20);

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnAddUserCollection, btnAddDirectoryCollection, btnCancel;
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
		if COMMANDEVENT(btnAddUserCollection)
		{
			ShowWindow(Window(), SW_HIDE);
			CollectionManager::addCollection<UserCollection>();
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnAddDirectoryCollection)
		{
			ShowWindow(Window(), SW_HIDE);
			CollectionManager::addCollection<DirectoryCollection>();
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
	addCollectionWindow->Create("wallhaven", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 200, 110, NULL, NULL);
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