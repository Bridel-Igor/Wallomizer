#include "SettingsWindow.h"
#include "Settings.h"
#include "MainWindow.h"

SettingsWindow* SettingsWindow::settingsWindow = nullptr;

LRESULT SettingsWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		stDelay = new Static(Window(), "Delay:",		10,		30,		80,		20, SS_RIGHT);
		stHours = new Static(Window(), "Hours",			100,	10,		74,		20, SS_CENTER);
		stMinutes = new Static(Window(), "Minutes",		183,	10,		74,		20, SS_CENTER);
		stSeconds = new Static(Window(), "Seconds",		266,	10,		74,		20, SS_CENTER);
		udeHours = new UpDownEdit(Window(),				100,	30,		74,		20, 0, 999, int((Settings::delay / 1000) / 3600));
		udeMinutes = new UpDownEdit(Window(),			183,	30,		74,		20, 0, 59, int((Settings::delay / 1000) / 60) % 60);
		udeSeconds = new UpDownEdit(Window(),			266,	30,		74,		20, 0, 59, int(Settings::delay / 1000) % 60);

		stUsername = new Static(Window(), "Username:",	10,		60,		80,		20, SS_RIGHT);
		edUsername = new Edit(Window(), "",				100,	60,		240,	20);

		stApiKey = new Static(Window(), "Api key:",		10,		90,		80,		20, SS_RIGHT);
		edApiKey = new Edit(Window(), "",				100,	90,		240,	20, ES_PASSWORD);

		btnCancel = new Button(Window(), "Cancel",		10,		120,	80,		20);
		btnOk = new Button(Window(), "Ok",				100,	120,	240,	20);

		edUsername->setTextA(Settings::username);
		edApiKey->setTextA(Settings::apiKey);

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnOk, btnCancel;
		delete stHours, stMinutes, stSeconds, stDelay, stApiKey, stUsername;
		delete edApiKey, edUsername;
		delete udeHours, udeMinutes, udeSeconds;
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
		if COMMANDEVENT(btnOk)
		{
			unsigned long delay = (udeSeconds->getPos() + (udeMinutes->getPos() * 60) + (udeHours->getPos() * 3600)) * 1000;

			if (delay < 5000)
			{
				MessageBoxA(nullptr, "Too small delay. Delay must be at least 5 seconds.", "wallhaven", MB_OK);
				return 0;
			}

			Settings::delay = delay;
			edUsername->getTextA(Settings::username, 64);
			edApiKey->getTextA(Settings::apiKey, 128);
			Settings::saveSettings();
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

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(129, 193, 193));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)bkBrush;
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(200, 200, 200));
		SetDCBrushColor(hdc, RGB(200, 200, 200));
		return (LRESULT)GetStockObject(DC_BRUSH);
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

void SettingsWindow::windowThread()
{
	if (settingsWindow)
	{
		SetForegroundWindow(settingsWindow->Window());
		return;
	}
	settingsWindow = new SettingsWindow;
	settingsWindow->Create("Settings", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL);
	settingsWindow->centerWindow(MainWindow::mainWindow->Window());
	ShowWindow(settingsWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(settingsWindow->Window(), SW_HIDE);
	settingsWindow->Destroy();
	delete settingsWindow;
	settingsWindow = nullptr;
}