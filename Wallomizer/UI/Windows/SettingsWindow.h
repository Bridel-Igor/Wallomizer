#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "UpDownEdit.h"
#include "Button.h"
#include "CheckBox.h"
#include "ColorPickerButton.h"

class WinUtils;
class Settings;

class SettingsWindow : public IWindow
{
public:
	SettingsWindow(HWND hCaller, const WinUtils& winUtils, Settings& settings);
	~SettingsWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 400;
	static constexpr int height = 370;

private:
	const WinUtils& m_winUtils;
	Settings& m_settings;

	HWND m_hCaller;
	Static stApplication, stSlideshow, stWallhaven;
	Static stVersion, stActVersion, stDeveloper, stActDeveloper, stHours, stMinutes, stSeconds, stDelay, stBckColor, stApiKey, stUsername, stStartup;
	Edit edApiKey, edUsername;
	UpDownEdit udeHours, udeMinutes, udeSeconds;
	Button btnOk, btnCancel, btnUpdate;
	CheckBox cbStartup;
	ColorPickerButton cpbBckColor;
};
