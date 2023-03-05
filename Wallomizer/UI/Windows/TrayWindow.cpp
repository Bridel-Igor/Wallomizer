#include <thread>
#include <Windows.h>
#include <VersionHelpers.h>

#include "TrayWindow.h"
#include "resource.h"
#include "Settings.h"
#include "Delay.h"

#define WM_NOTIFYICONMSG (WM_USER + 2)

TrayWindow *TrayWindow::s_pTrayWindow = nullptr;

static BOOL TrayMessage(HWND hDlg, DWORD dwMessage, UINT uID, HICON hIcon, LPCSTR pszTip)
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
		if (lstrcpynA(tnd.szTip, pszTip, sizeof(tnd.szTip)) == nullptr)
			return FALSE;
	}
	else
		tnd.szTip[0] = '\0';

	return Shell_NotifyIconA(dwMessage, &tnd);
}

TrayWindow::TrayWindow(CollectionManager* pCollectionManager) :
	IWindow("Wallomizer", "Tray Window Class", WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, 
												500,	500,	width,	height),
	m_pCollectionManager(pCollectionManager),
	btnSettings			(this, "Settings",	40,		60,		65,		20),
	btnExit				(this, "Exit",		115,	60,		65,		20),
	player				(this,				10,		10,
											10,		35,		200,	20, m_pCollectionManager, SS_CENTER)
{
	s_pTrayWindow = this;

	pszIDStatusIcon = MAKEINTRESOURCE(IDI_APP);
	hStatusIcon = LoadIcon(GetModuleHandleA(NULL), pszIDStatusIcon);
	TrayMessage(hWnd(), NIM_ADD, 1, hStatusIcon, "Wallomizer");

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	ShowWindow(hWnd(), SW_HIDE);
}

TrayWindow::~TrayWindow()
{
	s_pTrayWindow = nullptr;
	ShowWindow(hWnd(), SW_HIDE);
	DestroyIcon(hStatusIcon);

	Delay::exiting = true;
	Delay::saveSession(m_pCollectionManager->pCurrent);
	Delay::abortDelay();
}

LRESULT TrayWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NOTIFYICONMSG:
	{
		if (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN || lParam == WM_LBUTTONDBLCLK || lParam == WM_RBUTTONDBLCLK)
		{
			POINT pt;
			RECT rect;

			if (IsWindows7OrGreater())
			{
				NOTIFYICONIDENTIFIER nii;
				nii.cbSize = sizeof(NOTIFYICONIDENTIFIER);
				nii.hWnd = hWnd();
				nii.uID = 1;
				nii.guidItem = GUID_NULL;
				Shell_NotifyIconGetRect(&nii, &rect);

				pt.x = (int)((rect.left + rect.right) / 2);
				pt.y = (int)((rect.top + rect.bottom) / 2);
			}
			else
			{
				GetCursorPos(&pt);
			}

			pt.x -= int(width / 2);
			pt.y -= int(height / 2);

			SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);
			pt.x = pt.x				< rect.left		? rect.left				: pt.x;
			pt.x = pt.x + width		> rect.right	? rect.right - width		: pt.x;
			pt.y = pt.y				< rect.top		? rect.top				: pt.y;
			pt.y = pt.y + height	> rect.bottom	? rect.bottom - height	: pt.y;

			player.updateTimer(true);
			SetWindowPos(hWnd(), HWND_TOPMOST, pt.x, pt.y, width, height, SWP_SHOWWINDOW);
			SetForegroundWindow(hWnd());
			SendMessage(hWnd(), WM_SETCURSOR, 0, 0);
		}
	}
	return 0;

	case WM_NCACTIVATE:
	{
		if (wParam == FALSE)
		{
			ShowWindow(hWnd(), SW_HIDE);
			Shell_NotifyIconA(NIM_SETFOCUS, nullptr);
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player.draw(pDIS))
			return TRUE;
	}
	return 0;

	case WM_QUERYENDSESSION:
	{
		Delay::saveSession(m_pCollectionManager->pCurrent);
		return TRUE;
	}

	case WM_COMMAND:
	{
		if (player.click(wParam))
			return 0;
		if (btnSettings.isClicked(wParam))
		{
			m_pCollectionManager->openCollectionSettingsWindow();
			return 0;
		}
		if (btnExit.isClicked(wParam))
		{
			TrayMessage(hWnd(), NIM_DELETE, 1, hStatusIcon, "Wallomizer");
			DestroyWindow(hWnd());
			return 0;
		}
	}
	return 0;

	default:
		return RESULT_DEFAULT;
	}
}