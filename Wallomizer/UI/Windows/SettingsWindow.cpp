#include <shlobj_core.h>

#include "SettingsWindow.h"
#include "Settings.h"
#include "Player.h"

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszDirPath, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
	HRESULT hRes;
	IShellLink* psl;

	if (!SUCCEEDED(CoInitialize(NULL)))
		return -1;
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hRes))
	{
		IPersistFile* ppf;
		psl->SetPath(lpszPathObj);
		psl->SetDescription(lpszDesc);
		psl->SetWorkingDirectory(lpszDirPath);
		hRes = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
		if (SUCCEEDED(hRes))
		{
			WCHAR wsz[MAX_PATH];
			int ret = MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);
			if (ret != 0)
				hRes = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	CoUninitialize();
	return hRes;
}

SettingsWindow::SettingsWindow(HWND hCaller) :
	IWindow("Settings", "Setting Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_hCaller(hCaller),
	stApplication	(hWnd(), "Application",			10,		10,		380,	20, SS_CENTER),
	stVersion		(hWnd(), "Version:",			10,		40,		130,	20, SS_RIGHT),
	stActVersion	(hWnd(), "1.0.5a",				150,	40,		100,	20),
	btnUpdate		(hWnd(), "Check for updates",	270,	40,		120,	20),
	stStartup		(hWnd(), "Load on startup:",	10,		70,		130,	20, SS_RIGHT),
	cbStartup		(hWnd(),						150,	70,		20,		20),

	stSlideshow		(hWnd(), "Slideshow",			10,		100,	380,	20, SS_CENTER),
	stDelay			(hWnd(), "Delay:",				10,		150,	130,	20, SS_RIGHT),
	stHours			(hWnd(), "Hours",				150,	130,	74,		20, SS_CENTER),
	stMinutes		(hWnd(), "Minutes",				233,	130,	74,		20, SS_CENTER),
	stSeconds		(hWnd(), "Seconds",				316,	130,	74,		20, SS_CENTER),
	udeHours		(hWnd(),						150,	150,	74,		20, 0, 999, int((Settings::delay / 1000) / 3600)),
	udeMinutes		(hWnd(),						233,	150,	74,		20, 0, 59, int((Settings::delay / 1000) / 60) % 60),
	udeSeconds		(hWnd(),						316,	150,	74,		20, 0, 59, int(Settings::delay / 1000) % 60),

	stWallhaven		(hWnd(), "Wallhaven",			10,		180,	380,	20, SS_CENTER),
	stApiKey		(hWnd(), "Api key:",			10,		210,	130,	20, SS_RIGHT),
	edApiKey		(hWnd(), "",					150,	210,	240,	20, ES_PASSWORD),
	stUsername		(hWnd(), "Default username:",	10,		240,	130,	20, SS_RIGHT),
	edUsername		(hWnd(), "",					150,	240,	240,	20),

	btnCancel		(hWnd(), "Cancel",				10,		280,	130,	20),
	btnOk			(hWnd(), "Ok",					150,	280,	240,	20)
{
	EnableWindow(m_hCaller, FALSE);

	edUsername.setTextW(Settings::username);
	edApiKey.setTextW(Settings::apiKey);
	cbStartup.setChecked(Settings::loadOnStartup);

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	SendMessage(stApplication.hWnd(), WM_SETFONT, (WPARAM)resources.titleFont, TRUE);
	SendMessage(stSlideshow.hWnd(), WM_SETFONT, (WPARAM)resources.titleFont, TRUE);
	SendMessage(stWallhaven.hWnd(), WM_SETFONT, (WPARAM)resources.titleFont, TRUE);

	centerWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

SettingsWindow::~SettingsWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT SettingsWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_UPDATE)
		{
			if ((HWND)lParam == udeSeconds.m_edithWnd)
			{
				char buf[10];
				udeSeconds.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
				{
					udeSeconds.m_invalid = false;
				}
				else
					udeSeconds.m_invalid = true;
			}
			if ((HWND)lParam == udeMinutes.m_edithWnd)
			{
				char buf[10];
				udeMinutes.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
				{
					udeMinutes.m_invalid = false;
				}
				else
					udeMinutes.m_invalid = true;
			}
			if ((HWND)lParam == udeHours.m_edithWnd)
			{
				char buf[10];
				udeHours.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 999 && res >= 0)
				{
					udeHours.m_invalid = false;
				}
				else
					udeHours.m_invalid = true;
			}
		}
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			if ((HWND)lParam == udeSeconds.m_edithWnd)
			{
				char buf[10];
				udeSeconds.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
					udeSeconds.setPos(res);
				if (res > 59)
					udeSeconds.setPos(59);
				if (res < 0)
					udeSeconds.setPos(0);
			}
			if ((HWND)lParam == udeMinutes.m_edithWnd)
			{
				char buf[10];
				udeMinutes.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 59 && res >= 0)
					udeMinutes.setPos(res);
				if (res > 59)
					udeMinutes.setPos(59);
				if (res < 0)
					udeMinutes.setPos(0);
			}
			if ((HWND)lParam == udeHours.m_edithWnd)
			{
				char buf[10];
				udeHours.getTextA(buf, 10);
				int res = atoi(buf);
				if (res <= 999 && res >= 0)
					udeHours.setPos(res);
				if (res > 999)
					udeHours.setPos(999);
				if (res < 0)
					udeHours.setPos(0);
			}
		}
		if (btnOk.isClicked(wParam))
		{
			unsigned long delay = (udeSeconds.getPos() + (udeMinutes.getPos() * 60) + (udeHours.getPos() * 3600)) * 1000;

			if (delay < 10000)
			{
				MessageBoxA(nullptr, "Too small delay. Delay must be at least 10 seconds.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			Settings::delay = delay;
			edUsername.getTextW(Settings::username, 64);
			edApiKey.getTextW(Settings::apiKey, 128);

			char startupPath[260];
			HRESULT hr = SHGetFolderPathA(NULL, CSIDL_STARTUP, 0, NULL, startupPath); // if target win Vista and later use SHGetKnownFolderPath()
			strcat_s(startupPath, "\\Wallomizer.lnk");
			if (SUCCEEDED(hr))
			{
				if (cbStartup.isChecked())
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
			Player::updateTimer(true);
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnUpdate.isClicked(wParam))
		{
			ShellExecute(0, 0, "https://github.com/Bridel-Igor/Wallomizer/releases", 0, 0, SW_SHOW);
			return 0;
		}
		if (cbStartup.isClicked(wParam))
		{
			cbStartup.click();
			return 0;
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (cbStartup.draw(pDIS, resources.mainBkBrush))
			return TRUE;
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		if ((HWND)lParam == stApplication.hWnd() || (HWND)lParam == stSlideshow.hWnd() || (HWND)lParam == stWallhaven.hWnd())
			SetTextColor(hdcStatic, resources.titleFontColor);
		else
			SetTextColor(hdcStatic, resources.mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)resources.mainBkBrush;
	}

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, Edit::fontColor);
		SetBkColor(hdc, Edit::bkColor);
		SetDCBrushColor(hdc, Edit::bkColor);
		if ((HWND)lParam == udeSeconds.m_edithWnd && udeSeconds.m_invalid ||
			(HWND)lParam == udeMinutes.m_edithWnd && udeMinutes.m_invalid ||
			(HWND)lParam == udeHours.m_edithWnd && udeHours.m_invalid)
				SetBkColor(hdc, Edit::bkInvalidColor);
		return (LRESULT)GetStockObject(DC_BRUSH);
	}

	case WM_SETCURSOR:
	{
		cbStartup.mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return RESULT_DEFAULT;
	}
}