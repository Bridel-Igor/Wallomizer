#include <shlobj_core.h>

#include "SettingsWindow.h"
#include "Settings.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "ResPickerWindow.h"

SettingsWindow* SettingsWindow::settingsWindow = nullptr;

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszDirPath, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
	HRESULT hres;
	IShellLink* psl;

	if (!SUCCEEDED(CoInitialize(NULL)))
		return -1;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;
		psl->SetPath(lpszPathObj);
		psl->SetDescription(lpszDesc);
		psl->SetWorkingDirectory(lpszDirPath);
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];
			int ret = MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);
			if (ret != 0)
				hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return hres;
}

LRESULT SettingsWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);

		stApplication = new Static(this->hWnd(), "Application",		10,		10,		380,	20, SS_CENTER);
		stVersion = new Static(this->hWnd(), "Version:",			10,		40,		130,	20, SS_RIGHT);
		stActVersion = new Static(this->hWnd(), "1.0.5a",			150,	40,		100,	20);
		btnUpdate = new Button(this->hWnd(), "Check for updates",	270,	40,		120,	20);
		stStartup = new Static(this->hWnd(), "Load on startup:",	10,		70,		130,	20, SS_RIGHT);
		cbStartup = new CheckBox(this->hWnd(),						150,	70,		20,		20);
		
		stSlideshow = new Static(this->hWnd(), "Slideshow",			10,		100,	380,	20, SS_CENTER);
		stDelay = new Static(this->hWnd(), "Delay:",				10,		150,	130,	20, SS_RIGHT);
		stHours = new Static(this->hWnd(), "Hours",					150,	130,	74,		20, SS_CENTER);
		stMinutes = new Static(this->hWnd(), "Minutes",				233,	130,	74,		20, SS_CENTER);
		stSeconds = new Static(this->hWnd(), "Seconds",				316,	130,	74,		20, SS_CENTER);
		udeHours = new UpDownEdit(this->hWnd(),						150,	150,	74,		20, 0, 999, int((Settings::delay / 1000) / 3600));
		udeMinutes = new UpDownEdit(this->hWnd(),					233,	150,	74,		20, 0, 59, int((Settings::delay / 1000) / 60) % 60);
		udeSeconds = new UpDownEdit(this->hWnd(),					316,	150,	74,		20, 0, 59, int(Settings::delay / 1000) % 60);
			
		stWallhaven = new Static(this->hWnd(), "Wallhaven",			10,		180,	380,	20, SS_CENTER);
		stApiKey = new Static(this->hWnd(), "Api key:",				10,		210,	130,	20, SS_RIGHT);
		edApiKey = new Edit(this->hWnd(), "",						150,	210,	240,	20, ES_PASSWORD);
		stUsername = new Static(this->hWnd(), "Default username:",	10,		240,	130,	20, SS_RIGHT);
		edUsername = new Edit(this->hWnd(), "",						150,	240,	240,	20);
		
		btnCancel = new Button(this->hWnd(), "Cancel",				10,		280,	130,	20);
		btnOk = new Button(this->hWnd(), "Ok",						150,	280,	240,	20);

		edUsername->setTextA(Settings::username);
		edApiKey->setTextA(Settings::apiKey);
		cbStartup->setChecked(Settings::loadOnStartup);

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
		SendMessage(stApplication->hWnd(), WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
		SendMessage(stSlideshow->hWnd(), WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
		SendMessage(stWallhaven->hWnd(), WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
	}
	return 0;

	case WM_DESTROY:
	{
		delete stApplication;
		delete stSlideshow;
		delete stWallhaven;

		delete btnOk;
		delete btnCancel;
		delete btnUpdate;

		delete stHours;
		delete stMinutes;
		delete stSeconds;
		delete stDelay;
		delete stApiKey;
		delete stUsername;
		delete stStartup;
		delete stVersion;
		delete stActVersion;

		delete edApiKey;
		delete edUsername;

		delete udeHours;
		delete udeMinutes;
		delete udeSeconds;

		delete cbStartup;

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

	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_UPDATE)
		{
			if (udeSeconds != nullptr && (HWND)lParam == udeSeconds->m_edithWnd)
			{
				char buf[10];
				udeSeconds->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
				{
					udeSeconds->m_invalid = false;
				}
				else
					udeSeconds->m_invalid = true;
			}
			if (udeMinutes != nullptr && (HWND)lParam == udeMinutes->m_edithWnd)
			{
				char buf[10];
				udeMinutes->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
				{
					udeMinutes->m_invalid = false;
				}
				else
					udeMinutes->m_invalid = true;
			}
			if (udeHours != nullptr && (HWND)lParam == udeHours->m_edithWnd)
			{
				char buf[10];
				udeHours->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 999 && res >= 0)
				{
					udeHours->m_invalid = false;
				}
				else
					udeHours->m_invalid = true;
			}
		}
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			if (udeSeconds != nullptr && (HWND)lParam == udeSeconds->m_edithWnd)
			{
				char buf[10];
				udeSeconds->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
					udeSeconds->setPos(res);
				if (res > 59)
					udeSeconds->setPos(59);
				if (res < 0)
					udeSeconds->setPos(0);
			}
			if (udeMinutes != nullptr && (HWND)lParam == udeMinutes->m_edithWnd)
			{
				char buf[10];
				udeMinutes->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
					udeMinutes->setPos(res);
				if (res > 59)
					udeMinutes->setPos(59);
				if (res < 0)
					udeMinutes->setPos(0);
			}
			if (udeHours != nullptr && (HWND)lParam == udeHours->m_edithWnd)
			{
				char buf[10];
				udeHours->getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 999 && res >= 0)
					udeHours->setPos(res);
				if (res > 999)
					udeHours->setPos(999);
				if (res < 0)
					udeHours->setPos(0);
			}
		}
		if (btnOk->isClicked(wParam))
		{
			unsigned long delay = (udeSeconds->getPos() + (udeMinutes->getPos() * 60) + (udeHours->getPos() * 3600)) * 1000;

			if (delay < 10000)
			{
				MessageBoxA(nullptr, "Too small delay. Delay must be at least 10 seconds.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			Settings::delay = delay;
			edUsername->getTextA(Settings::username, 64);
			edApiKey->getTextA(Settings::apiKey, 128);

			char startupPath[260];
			HRESULT hr = SHGetFolderPathA(NULL, CSIDL_STARTUP, 0, NULL, startupPath); // if target win Vista and later use SHGetKnownFolderPath()
			strcat_s(startupPath, "\\Wallomizer.lnk");
			if (SUCCEEDED(hr))
			{
				if (cbStartup->isChecked())
				{
					char currentPath[260], currentDirectory[260];
					GetModuleFileNameA(NULL, currentPath, 260);
					GetCurrentDirectoryA(260, currentDirectory);
					CreateLink(currentPath, currentDirectory, startupPath, "");
					Settings::loadOnStartup = true;
				}
				else
				{
					remove(startupPath);
					Settings::loadOnStartup = false;
				}
			}

			Settings::saveSettings();
			if (MainWindow::mainWindow && MainWindow::isReady())
				MainWindow::mainWindow->player->updateTimer();
			if (TrayWindow::trayWindow && TrayWindow::trayWindow->isReady())
				TrayWindow::trayWindow->player->updateTimer();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnCancel->isClicked(wParam))
		{
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnUpdate->isClicked(wParam))
		{
			ShellExecute(0, 0, "https://github.com/Bridel-Igor/Wallomizer/releases", 0, 0, SW_SHOW);
			return 0;
		}
		if (cbStartup->isClicked(wParam))
		{
			cbStartup->click();
			return 0;
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (cbStartup->draw(pDIS, WindowStyles::mainBkBrush))
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

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		if ((HWND)lParam == stApplication->hWnd() || (HWND)lParam == stSlideshow->hWnd() || (HWND)lParam == stWallhaven->hWnd())
			SetTextColor(hdcStatic, WindowStyles::titleFontColor);
		else
			SetTextColor(hdcStatic, WindowStyles::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)WindowStyles::mainBkBrush;
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, WindowStyles::editFontColor);
		SetBkColor(hdc, WindowStyles::editBkColor);
		SetDCBrushColor(hdc, WindowStyles::editBkColor);
		if ((udeSeconds != nullptr && (HWND)lParam == udeSeconds->m_edithWnd && udeSeconds->m_invalid) ||
			(udeMinutes != nullptr && (HWND)lParam == udeMinutes->m_edithWnd && udeMinutes->m_invalid) ||
			(udeHours != nullptr && (HWND)lParam == udeHours->m_edithWnd && udeHours->m_invalid))
				SetBkColor(hdc, WindowStyles::editBkInvalidColor);
		return (LRESULT)GetStockObject(DC_BRUSH);
	}
	return 0;

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}

	case WM_SETCURSOR:
	{
		cbStartup->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void SettingsWindow::windowThread()
{
	if (settingsWindow)
	{
		SetForegroundWindow(settingsWindow->hWnd());
		return;
	}
	settingsWindow = new SettingsWindow;
	settingsWindow->Create("Settings", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height, NULL, NULL, false);
	settingsWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(settingsWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(settingsWindow->hWnd(), SW_HIDE);
	settingsWindow->Destroy();
	delete settingsWindow;
	settingsWindow = nullptr;
}