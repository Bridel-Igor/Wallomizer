#include <shlobj_core.h>

#include "SettingsWindow.h"
#include "Settings.h"
#include "MainWindow.h"
#include "ResPickerWindow.h"

SettingsWindow* SettingsWindow::settingsWindow = nullptr;

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc)
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

		stDelay = new Static(Window(), "Delay:",		10,		30,		80,		20, SS_RIGHT);
		stHours = new Static(Window(), "Hours",			100,	10,		74,		20, SS_CENTER);
		stMinutes = new Static(Window(), "Minutes",		183,	10,		74,		20, SS_CENTER);
		stSeconds = new Static(Window(), "Seconds",		266,	10,		74,		20, SS_CENTER);
		udeHours = new UpDownEdit(Window(),				100,	30,		74,		20, 0, 999, int((Settings::delay / 1000) / 3600));
		udeMinutes = new UpDownEdit(Window(),			183,	30,		74,		20, 0, 59, int((Settings::delay / 1000) / 60) % 60);
		udeSeconds = new UpDownEdit(Window(),			266,	30,		74,		20, 0, 59, int(Settings::delay / 1000) % 60);
		
		stStartup = new Static(Window(), "Load on startup",120,	60,		100,	20);
		cbStartup = new CheckBox(Window(), "",			100,	60,		20,		20, NULL);

		stUsername = new Static(Window(), "Username:",	10,		90,		80,		20, SS_RIGHT);
		edUsername = new Edit(Window(), "",				100,	90,		240,	20);

		stApiKey = new Static(Window(), "Api key:",		10,		120,	80,		20, SS_RIGHT);
		edApiKey = new Edit(Window(), "",				100,	120,	240,	20, ES_PASSWORD);

		btnCancel = new Button(Window(), "Cancel",		10,		150,	80,		20);
		btnOk = new Button(Window(), "Ok",				100,	150,	240,	20);

		edUsername->setTextA(Settings::username);
		edApiKey->setTextA(Settings::apiKey);
		cbStartup->setChecked(Settings::loadOnStartup);

		EnumChildWindows(Window(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		delete btnOk, btnCancel;
		delete stHours, stMinutes, stSeconds, stDelay, stApiKey, stUsername, stStartup;
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
					char currentPath[260];
					GetModuleFileNameA(NULL, currentPath, 260);
					CreateLink(currentPath, startupPath, "");
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