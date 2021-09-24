#include "TrayWindow.h"
#include "resource.h"
#include "MainWindow.h"
#include "Settings.h"
#include "CollectionManager.h"

#define WM_NOTIFYICONMSG (WM_USER + 2)

TrayWindow *TrayWindow::trayWindow = nullptr;

BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, LPCSTR pszTip)
{
	NOTIFYICONDATA tnd;

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = hDlg;
	tnd.uID = uID;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICONMSG;
	tnd.hIcon = hIcon;

	if (pszTip)
	{
		if (lstrcpynA(tnd.szTip, pszTip, sizeof(tnd.szTip))==nullptr)
			return FALSE;
	}
	else
		tnd.szTip[0] = '\0';

	return Shell_NotifyIconA(dwMessage, &tnd);
}

LRESULT TrayWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NOTIFYICONMSG && (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN))
	{
		hPopup = GetSubMenu(hMenu, 0);
		if (CollectionManager::isPrevious())
			EnableMenuItem(hPopup, 3, MF_BYPOSITION | MF_ENABLED);
		else
			EnableMenuItem(hPopup, 3, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(hPopup, 0, Settings::bRunSlideshow ? MF_BYPOSITION | MF_DISABLED | MF_GRAYED : MF_BYPOSITION | MF_ENABLED);
		EnableMenuItem(hPopup, 1, Settings::bRunSlideshow ? MF_BYPOSITION | MF_ENABLED : MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		SetForegroundWindow(hWnd);
		POINT pt;
		GetCursorPos(&pt);
		TrackPopupMenu(hPopup, 0, pt.x, pt.y, 0, hWnd, NULL);
	}

	switch (uMsg)
	{
	case WM_CREATE:
	{		
		pszIDStatusIcon = MAKEINTRESOURCE(IDI_ICON1);
		hStatusIcon = LoadIcon(GetModuleHandleA(NULL), pszIDStatusIcon);
		TrayMessage(hWnd, NIM_ADD, 1, hStatusIcon, "Wallhaven");
		hMenu = LoadMenu(GetModuleHandleA(NULL), MAKEINTRESOURCE(IDR_MENU1));
	}
	return 0;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ID_WALLHAVEN_START:
			Settings::startSlideshow();
			if (MainWindow::mainWindow)
				InvalidateRect(MainWindow::mainWindow->Window(), NULL, FALSE);
			break;
		case ID_WALLHAVEN_PAUSE:
			Settings::pauseSlideshow();
			if (MainWindow::mainWindow)
				InvalidateRect(MainWindow::mainWindow->Window(), NULL, FALSE);
			break;
		case ID_WALLHAVEN_NEXTWALLPAPER:
			Settings::replayDelay();
			CollectionManager::setNextWallpaper();
			break;
		case ID_WALLHAVEN_PREVIOUSWALLPAPER:
			Settings::replayDelay();
			CollectionManager::setPreviousWallpaper();
			break;
		case ID_WALLHAVEN_SETTINGS:
		{
			std::thread thr(MainWindow::windowThread);
			thr.detach();
			break;
		}
		case ID_WALLHAVEN_OPENWALLPAPER:
		{
			CollectionManager::openWallpaperExternal();
			break;
		}
		case ID_WALLHAVEN_EXIT:
			TrayMessage(hWnd, NIM_DELETE, 1, hStatusIcon, "Wallhaven");
			DestroyMenu(hMenu);
			DestroyWindow(hWnd);
			break;
		}
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void TrayWindow::windowThread()
{
	if (trayWindow)
		return;
	trayWindow = new TrayWindow;
	trayWindow->Create("Wallhaven", NULL, NULL, 0, 0, 0, 0, NULL, NULL);
	ShowWindow(trayWindow->Window(), SW_HIDE);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	trayWindow->Destroy();
	delete trayWindow;
	Settings::exiting = true;
	Settings::abortDelay();
	trayWindow = nullptr;
}