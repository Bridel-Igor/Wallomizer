#include "TrayWindow.h"

#include <Windows.h>

#include "App.h"

TrayWindow::TrayWindow(App& app, POINT pt) :
	IWindow(nullptr, "Wallomizer", "Tray Window Class", WS_POPUP | WS_BORDER, WS_EX_TOOLWINDOW, 
											500,	500,	width,	height),
	m_app(app),
	btnSettings			(this, "Settings",	40,		60,		65,		20),
	btnExit				(this, "Exit",		115,	60,		65,		20),
	player				(this,				10,		10,
											10,		35,		200,	20, m_app.getWinUtils(), m_app.getTimer(), m_app.getWallpaperManager(), SS_CENTER)
{
	positionWindow(pt);
	SetForegroundWindow(hWnd());
	SendMessage(hWnd(), WM_SETCURSOR, 0, 0);
}

LRESULT TrayWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCACTIVATE:
		if (wParam == FALSE)
			DestroyWindow(hWnd());
		break;

	case WM_DRAWITEM:
		if (player.draw(reinterpret_cast<LPDRAWITEMSTRUCT>(lParam)))
			return TRUE;
		break;

	case WM_COMMAND:
		if (player.click(wParam))
			return 0;
		if (btnSettings.isClicked(wParam))
		{
			m_app.getUI().openMainWindowAsync();
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnExit.isClicked(wParam))
		{
			m_app.requestExit();
			return 0;
		}
		return 0;
		break;
	}
	
	return RESULT_DEFAULT;
}

void TrayWindow::positionWindow(POINT pt)
{
	HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo{};
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfoA(hMonitor, &monitorInfo);
	const RECT workArea = monitorInfo.rcWork;

	pt.x -= width / 2;
	pt.x = pt.x < workArea.left ? workArea.left : pt.x;
	pt.x = pt.x + width > workArea.right ? workArea.right - width : pt.x;

	pt.y -= height / 2;
	pt.y = pt.y < workArea.top ? workArea.top : pt.y;
	pt.y = pt.y + height > workArea.bottom ? workArea.bottom - height : pt.y;

	SetWindowPos(hWnd(), HWND_TOPMOST, pt.x, pt.y, width, height, SWP_SHOWWINDOW);
}
