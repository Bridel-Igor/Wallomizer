#include <thread>

#include "TrayWindow.h"
#include "resource.h"
#include "MainWindow.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Delay.h"

#define WM_NOTIFYICONMSG (WM_USER + 2)

TrayWindow *TrayWindow::trayWindow = nullptr;
bool TrayWindow::s_isReady = false;

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
	switch (uMsg)
	{
	case WM_NOTIFYICONMSG:
	{	
		if (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN)
		{
			POINT pt;
			RECT wrkArea;

			GetCursorPos(&pt);
			pt.x -= int(width / 2);
			pt.y -= int(height / 2);

			SystemParametersInfoA(SPI_GETWORKAREA, 0, &wrkArea, 0);
			pt.x = pt.x - int(width / 2) < wrkArea.left		? wrkArea.left				: pt.x;
			pt.x = pt.x + int(width / 2) > wrkArea.right	? wrkArea.right - width		: pt.x;
			pt.y = pt.y - int(height / 2) < wrkArea.top		? wrkArea.top				: pt.y;
			pt.y = pt.y + int(height / 2) > wrkArea.bottom	? wrkArea.bottom - height	: pt.y;

			player->updateTimer(true);
			player->redrawPlayers();
			SetWindowPos(trayWindow->hWnd(), HWND_TOPMOST, pt.x, pt.y, width, height, SWP_SHOWWINDOW);
			SetForegroundWindow(trayWindow->hWnd());
		}
	}
	return 0;

	case WM_NCACTIVATE:
	{// BUG: window isn't appearing sometimes
		if (wParam == FALSE)
			ShowWindow(trayWindow->hWnd(), SW_HIDE);
	}
	return 0;

	case WM_CREATE:
	{		
		pszIDStatusIcon = MAKEINTRESOURCE(IDI_APP);
		hStatusIcon = LoadIcon(GetModuleHandleA(NULL), pszIDStatusIcon);
		TrayMessage(hWnd, NIM_ADD, 1, hStatusIcon, "Wallomizer");

		player = new Player(this->hWnd(),					10,		10,
															10,		35,		140,	20, SS_CENTER);
			
		btnSettings = new Button(this->hWnd(), "Settings",	10,		60,		65,		20);
		btnExit = new Button(this->hWnd(), "Exit",			85,		60,		65,		20);

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);

		s_isReady = true;
	}
	return 0;

	case WM_DESTROY:
	{
		s_isReady = false;

		delete btnSettings;
		delete btnExit;
		delete player;
		DestroyIcon(hStatusIcon);
		PostQuitMessage(0);
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player->draw(pDIS))
			return TRUE;
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

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;

	case WM_QUERYENDSESSION:
	{
		Delay::saveSession(CollectionManager::current);
		return TRUE;
	}
	return 0;

	case WM_COMMAND:
	{
		if (player->click(wParam))
			return 0;
		if (btnSettings->isClicked(wParam))
		{
			std::thread thr(MainWindow::windowThread);
			thr.detach();
			return 0;
		}
		if (btnExit->isClicked(wParam))
		{
			TrayMessage(hWnd, NIM_DELETE, 1, hStatusIcon, "Wallomizer");
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		player->mouseHovering(wParam);
		return FALSE;
	}
	return 0;

	default:
		return DefWindowProcA(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void TrayWindow::windowThread()
{
	if (trayWindow)
		return;
	trayWindow = new TrayWindow;
	trayWindow->Create("Wallomizer", WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, 500, 500, width, height, NULL, NULL);
	ShowWindow(trayWindow->hWnd(), SW_HIDE);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(trayWindow->hWnd(), SW_HIDE);
	trayWindow->Destroy();
	delete trayWindow;
	Delay::exiting = true;
	Delay::saveSession(CollectionManager::current);
	Delay::abortDelay();
	trayWindow = nullptr;
}

bool TrayWindow::isReady()
{
	return s_isReady;
}