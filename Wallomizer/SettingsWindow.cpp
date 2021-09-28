#include <shlobj_core.h>

#include "SettingsWindow.h"
#include "Settings.h"
#include "MainWindow.h"
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
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		stApplication = new Static(Window(), "Application",		10,		10,		380,	20, SS_CENTER);
		stVersion = new Static(Window(), "Version:",			10,		40,		130,	20, SS_RIGHT);
		stActVersion = new Static(Window(), "1.0.2",			150,	40,		100,	20);
		btnUpdate = new Button(Window(), "Check for updates",	270,	40,		120,	20);
		stStartup = new Static(Window(), "Load on startup:",	10,		70,		130,	20, SS_RIGHT);
		cbStartup = new CheckBox(Window(), "",					150,	70,		20,		20, NULL);
		
		stSlideshow = new Static(Window(), "Slideshow",			10,		100,	380,	20, SS_CENTER);
		stDelay = new Static(Window(), "Delay:",				10,		150,	130,	20, SS_RIGHT);
		stHours = new Static(Window(), "Hours",					150,	130,	74,		20, SS_CENTER);
		stMinutes = new Static(Window(), "Minutes",				233,	130,	74,		20, SS_CENTER);
		stSeconds = new Static(Window(), "Seconds",				316,	130,	74,		20, SS_CENTER);
		udeHours = new UpDownEdit(Window(),						150,	150,	74,		20, 0, 999, int((Settings::delay / 1000) / 3600));
		udeMinutes = new UpDownEdit(Window(),					233,	150,	74,		20, 0, 59, int((Settings::delay / 1000) / 60) % 60);
		udeSeconds = new UpDownEdit(Window(),					316,	150,	74,		20, 0, 59, int(Settings::delay / 1000) % 60);
			
		stWallhaven = new Static(Window(), "Wallhaven",			10,		180,	380,	20, SS_CENTER);
		stApiKey = new Static(Window(), "Api key:",				10,		210,	130,	20, SS_RIGHT);
		edApiKey = new Edit(Window(), "",						150,	210,	240,	20, ES_PASSWORD);
		stUsername = new Static(Window(), "Default username:",	10,		240,	130,	20, SS_RIGHT);
		edUsername = new Edit(Window(), "",						150,	240,	240,	20);
		
		btnCancel = new Button(Window(), "Cancel",				10,		280,	130,	20);
		btnOk = new Button(Window(), "Ok",						150,	280,	240,	20);

		edUsername->setTextA(Settings::username);
		edApiKey->setTextA(Settings::apiKey);
		cbStartup->setChecked(Settings::loadOnStartup);

		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);
		SendMessage(stApplication->hWnd, WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
		SendMessage(stSlideshow->hWnd, WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
		SendMessage(stWallhaven->hWnd, WM_SETFONT, (WPARAM)WindowStyles::titleFont, TRUE);
	}
	return 0;

	case WM_DESTROY:
	{
		delete stApplication, stSlideshow, stWallhaven;
		delete btnOk, btnCancel, btnUpdate;
		delete stHours, stMinutes, stSeconds, stDelay, stApiKey, stUsername, stStartup, stVersion, stActVersion;
		delete edApiKey, edUsername;
		delete udeHours, udeMinutes, udeSeconds;
		delete cbStartup;

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

	case WM_COMMAND:
	{
		if COMMANDEVENT(btnOk)
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
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnUpdate)
		{
			ShellExecute(0, 0, "https://github.com/Bridel-Igor/Wallomizer/releases", 0, 0, SW_SHOW);
			return 0;
		}
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
		if ((HWND)lParam == stApplication->hWnd || (HWND)lParam == stSlideshow->hWnd || (HWND)lParam == stWallhaven->hWnd)
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