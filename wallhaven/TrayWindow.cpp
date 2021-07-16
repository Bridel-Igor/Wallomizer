#include "TrayWindow.h"
#include "resource.h"
#include "SettingsWindow.h"
#include "Settings.h"

#define WM_NOTIFYICONMSG (WM_USER + 2)

TrayWindow *TrayWindow::trayWindow = nullptr;

BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, PSTR pszTip)
{
	NOTIFYICONDATA tnd;

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = hDlg;
	tnd.uID = uID;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICONMSG;
	tnd.hIcon = hIcon;

	if (pszTip)
		lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
	else
		tnd.szTip[0] = '\0';

	return Shell_NotifyIconA(dwMessage, &tnd);
}

BOOL TrayWindow::ShowPopupMenu(HWND hWnd, HINSTANCE hInstance, WORD nResourceID)
{
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(nResourceID));
	if (!hMenu)
		return FALSE;
	hPopup = GetSubMenu(hMenu, 0);
	if (!hPopup)
		return FALSE;

	SetForegroundWindow(hWnd);

	POINT pt;
	GetCursorPos(&pt);
	BOOL bOK = TrackPopupMenu(hPopup, 0, pt.x, pt.y, 0, hWnd, NULL);

	DestroyMenu(hMenu);
	return bOK;
}

LRESULT TrayWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NOTIFYICONMSG && lParam == WM_RBUTTONDOWN)
	{
		ShowPopupMenu(hWnd, GetModuleHandleA(NULL), IDR_MENU1);
	}

	switch (uMsg)
	{
	case WM_CREATE:
	{		
		HICON hStatusIcon;
		LPCSTR pszIDStatusIcon;
		NOTIFYICONDATA tnd;

		pszIDStatusIcon = MAKEINTRESOURCE(IDI_ICON1);

		hStatusIcon = LoadIcon(GetModuleHandleA(NULL), pszIDStatusIcon);
		tnd.cbSize = sizeof(NOTIFYICONDATA);
		tnd.hWnd = m_hWnd;
		tnd.uID = 1;
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = WM_NOTIFYICONMSG;
		tnd.hIcon = hStatusIcon;
		lstrcpyn(tnd.szTip, "wallhaven", sizeof(tnd.szTip));
		Shell_NotifyIcon(NIM_ADD, &tnd);
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
			if (!slideshow->try_lock())
				slideshow->unlock();
			break;
		case ID_WALLHAVEN_NEXTWALLPAPER:
			Settings::abortDelay();
			break;
		case ID_WALLHAVEN_EXIT:
			TrayMessage(hWnd, NIM_DELETE, 0, 0, 0);
			DestroyWindow(hWnd);			
			exit(0);
			break;
		case ID_WALLHAVEN_SETTINGS:
			std::thread thr(SettingsWindow::windowThread);
			thr.detach();
			break;
		}
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void TrayWindow::windowThread(std::mutex *slideshow)
{
	if (trayWindow)
		return;
	trayWindow = new TrayWindow;
	trayWindow->slideshow = slideshow;
	trayWindow->Create("wallhaven", NULL, NULL, 0, 0, 0, 0, NULL, NULL);
	ShowWindow(trayWindow->Window(), SW_HIDE);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	trayWindow->Destroy();
	delete trayWindow;
	trayWindow = nullptr;
}