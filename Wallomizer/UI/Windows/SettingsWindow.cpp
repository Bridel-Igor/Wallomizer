#include "SettingsWindow.h"

#include <Shellapi.h>

#include "WinUtils.h"
#include "Settings.h"
#include "Player.h"
#include "Internet.h"

SettingsWindow::SettingsWindow(HWND hCaller, const WinUtils& winUtils, Settings& settings) :
	IWindow("Settings", "Setting Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_winUtils(winUtils),
	m_settings(settings),
	m_hCaller(hCaller),
	stApplication	(this, "Application",					10,		10,		380,	20, SS_CENTER),
	stVersion		(this, "Version:",						10,		40,		130,	20, SS_RIGHT),
	stActVersion	(this, "",								150,	40,		100,	20),
	btnUpdate		(this, "Check for updates",				270,	40,		120,	20),
	stDeveloper		(this, "Developer:",					10,		70,		130,	20, SS_RIGHT),
	stActDeveloper	(this, "Igor Bridel",					150,	70,		100,	20),
	stStartup		(this, "Load on startup:",				10,		100,	130,	20, SS_RIGHT),
	cbStartup		(this,									150,	100,	20,		20),

	stSlideshow		(this, "Slideshow",						10,		130,	380,	20, SS_CENTER),
	stDelay			(this, "Delay:",						10,		180,	130,	20, SS_RIGHT),
	stHours			(this, "Hours",							150,	160,	74,		20, SS_CENTER),
	stMinutes		(this, "Minutes",						233,	160,	74,		20, SS_CENTER),
	stSeconds		(this, "Seconds",						316,	160,	74,		20, SS_CENTER),
	udeHours		(this,									150,	180,	74,		20, 0, 999, int((m_settings.getData().delay / 1000) / 3600)),
	udeMinutes		(this,									233,	180,	74,		20, 0, 59, int((m_settings.getData().delay / 1000) / 60) % 60),
	udeSeconds		(this,									316,	180,	74,		20, 0, 59, int(m_settings.getData().delay / 1000) % 60),

	stBckColor		(this, "Background color:",				10,		210,	130,	20,	SS_RIGHT),
	cpbBckColor		(this, m_winUtils.getBackgroundColor(),	150,	210,	120,	20),

	stWallhaven		(this, "Wallhaven",						10,		240,	380,	20, SS_CENTER),
	stApiKey		(this, "Api key:",						10,		270,	130,	20, SS_RIGHT),
	edApiKey		(this, "",								150,	270,	240,	20, ES_PASSWORD),
	stUsername		(this, "Default username:",				10,		300,	130,	20, SS_RIGHT),
	edUsername		(this, "",								150,	300,	240,	20),

	btnCancel		(this, "Cancel",						10,		340,	130,	20),
	btnOk			(this, "Ok",							150,	340,	240,	20)
{
	EnableWindow(m_hCaller, FALSE);

	SetWindowText(stActVersion.hWnd(), m_winUtils.getAppVersion().c_str());

	edUsername.setTextW(m_settings.getData().username.c_str());
	edApiKey.setTextW(m_settings.getData().apiKey.c_str());
	cbStartup.setChecked(m_settings.getData().loadOnStartup);

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
			Settings::Data& data = m_settings.getData();
			Settings::Data newData = data;
			newData.loadOnStartup = cbStartup.isChecked();
			newData.delay = (udeSeconds.getPos() + (udeMinutes.getPos() * 60) + (udeHours.getPos() * 3600)) * 1000;
			wchar_t apiKey[33], username[64];
			edApiKey.getTextW(apiKey, 33);
			edUsername.getTextW(username, 64);
			newData.apiKey = apiKey;
			newData.username = username;

			if (!newData.validateDelay())
			{
				MessageBoxA(nullptr, "Delay must be at least 10 seconds.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			if (!newData.validateApiKeyLength())
			{
				MessageBoxA(nullptr, "Invalid API key. It must be 32 characters long.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			if (!newData.validateUsername())
			{
				MessageBoxA(nullptr, "User name is too long.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			
			Internet internet;
			std::wstring apiKeyTestUrl = L"https://wallhaven.cc/api/v1/settings?apikey=";
			apiKeyTestUrl += newData.apiKey;
			internet.downloadToBuffer(apiKeyTestUrl);
			if (internet.parse("error"))
			{
				MessageBoxA(nullptr, "Unknown API key!", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			if (!internet.parse("data"))
				MessageBoxA(nullptr, "Can't check API key! Wallhaven API is down, or no internet connection.", "Wallomizer", MB_OK | MB_ICONINFORMATION);

			Settings::Data backupData = data;
			data = newData;
			if (m_settings.saveSettings() == false)
			{
				data = backupData;
				MessageBoxA(nullptr, "Unable to save settings.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}

			m_winUtils.setStartup(cbStartup.isChecked());
			m_winUtils.setBackgroundColor(cpbBckColor.getColor());
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
		if (cpbBckColor.isClicked(wParam))
		{
			cpbBckColor.click();
			return 0;
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (cbStartup.draw(pDIS, resources.mainBkBrush))
			return TRUE;
		if (cpbBckColor.draw(pDIS))
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

	default:
		return RESULT_DEFAULT;
	}
}
