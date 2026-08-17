#include "SettingsWindow.h"

#include <Shellapi.h>

#include "WinUtils.h"
#include "Settings.h"
#include "Player.h"
#include "Internet.h"
#include "UIColor.h"

SettingsWindow::SettingsWindow(IWindow* pOwner, const WinUtils& winUtils, Settings& settings) :
	IWindow(pOwner, "Settings", "Setting Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, width, height),
	m_winUtils(winUtils),
	m_settings(settings),
	stApplication	(this, "Application",					10,		10,		380,	20, SS_CENTER),
	stVersion		(this, "Version:",						10,		40,		130,	20, SS_RIGHT),
	stActVersion	(this, m_winUtils.getAppVersion(),		150,	40,		100,	20),
	btnUpdate		(this, "Check for updates",				270,	40,		120,	20),
	stDeveloper		(this, "Developer:",					10,		70,		130,	20, SS_RIGHT),
	stActDeveloper	(this, "Igor Bridel",					150,	70,		100,	20),
	stStartup		(this, "Load on startup:",				10,		100,	130,	20, SS_RIGHT),
	cbStartup		(this,									150,	100,	20,		20, m_settings.getData().loadOnStartup),

	stSlideshow		(this, "Slideshow",						10,		130,	380,	20, SS_CENTER),
	stDelay			(this, "Delay:",						10,		180,	130,	20, SS_RIGHT),
	stHours			(this, "Hours",							150,	160,	74,		20, SS_CENTER),
	stMinutes		(this, "Minutes",						233,	160,	74,		20, SS_CENTER),
	stSeconds		(this, "Seconds",						316,	160,	74,		20, SS_CENTER),
	udeHours		(this,									150,	180,	74,		20, 0, 999, int((m_settings.getData().delay / 1000) / 3600)),
	udeMinutes		(this,									233,	180,	74,		20, 0, 59, int((m_settings.getData().delay / 1000) / 60) % 60),
	udeSeconds		(this,									316,	180,	74,		20, 0, 59, int(m_settings.getData().delay / 1000) % 60),

	stBckColor		(this, "Background color:",				10,		210,	130,	20, SS_RIGHT),
	cpbBckColor		(this, m_winUtils.getBackgroundColor(),	150,	210,	120,	20),

	stWallhaven		(this, "Wallhaven",						10,		240,	380,	20, SS_CENTER),
	stApiKey		(this, "Api key:",						10,		270,	130,	20, SS_RIGHT),
	edApiKey		(this, m_settings.getData().apiKey,		150,	270,	240,	20, ES_PASSWORD),
	stUsername		(this, "Default username:",				10,		300,	130,	20, SS_RIGHT),
	edUsername		(this, m_settings.getData().username,	150,	300,	240,	20),

	btnCancel		(this, "Cancel",						10,		340,	130,	20),
	btnOk			(this, "Ok",							150,	340,	240,	20)
{}

LRESULT SettingsWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		stApplication.setFont(resources.titleFont);
		stSlideshow.setFont(resources.titleFont);
		stWallhaven.setFont(resources.titleFont);
		break;
	}

	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			HWND hEdit = reinterpret_cast<HWND>(lParam);
			if (hEdit == udeSeconds.m_editHWnd)
			{
				udeSeconds.syncEdit();
				return 0;
			}
			if (hEdit == udeMinutes.m_editHWnd)
			{
				udeMinutes.syncEdit();
				return 0;
			}
			if (hEdit == udeHours.m_editHWnd)
			{
				udeHours.syncEdit();
				return 0;
			}
			break;
		}
		if (btnOk.isClicked(wParam))
		{
			Settings::Data& data = m_settings.getData();
			Settings::Data newData = data;
			newData.loadOnStartup = cbStartup.isChecked();
			newData.delay = (udeSeconds.getPos() + (udeMinutes.getPos() * 60) + (udeHours.getPos() * 3600)) * 1000;
			newData.username = edUsername.textW();
			newData.apiKey = edApiKey.textW();

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
			
			if (!newData.apiKey.empty())
			{
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
			}

			Settings::Data backupData = data;
			data = newData;
			if (!m_settings.saveSettings())
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
			ShellExecuteA(nullptr, nullptr, "https://github.com/Bridel-Igor/Wallomizer/releases", nullptr, nullptr, SW_SHOW);
			return 0;
		}
		if (cbStartup.isClicked(wParam))
		{
			cbStartup.toggle();
			return 0;
		}
		if (cpbBckColor.isClicked(wParam))
		{
			cpbBckColor.click();
			return 0;
		}
		break;
	}

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (cbStartup.draw(drawItem, resources.mainBkBrush))
			return TRUE;
		if (cpbBckColor.draw(drawItem))
			return TRUE;
		break;
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		if ((HWND)lParam == stApplication.hWnd() || (HWND)lParam == stSlideshow.hWnd() || (HWND)lParam == stWallhaven.hWnd())
			SetTextColor(hdcStatic, UIColor::staticTitle);
		else
			SetTextColor(hdcStatic, UIColor::staticText);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)resources.mainBkBrush;
	}

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, UIColor::editText);
		SetBkColor(hdc, UIColor::editBk);
		SetDCBrushColor(hdc, UIColor::editBk);

		HWND hEdit = reinterpret_cast<HWND>(lParam);

		if (((hEdit == udeSeconds.m_editHWnd) && !udeSeconds.isEditValid()) ||
			((hEdit == udeMinutes.m_editHWnd) && !udeMinutes.isEditValid()) ||
			((hEdit == udeHours.m_editHWnd) && !udeHours.isEditValid()))
				SetBkColor(hdc, UIColor::editInvalidBk);
		return (LRESULT)GetStockObject(DC_BRUSH);
	}
	}
	
	return RESULT_DEFAULT;
}
