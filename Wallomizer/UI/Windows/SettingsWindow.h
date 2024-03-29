#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CheckBox.h"
#include "ColorPickerButton.h"

class SettingsWindow : public IWindow
{
public:
	SettingsWindow(HWND hCaller);
	~SettingsWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 400;
	static constexpr int height = 370;

private:
	HWND m_hCaller;
	Static stApplication, stSlideshow, stWallhaven;
	Static stVersion, stActVersion, stDeveloper, stActDeveloper, stHours, stMinutes, stSeconds, stDelay, stBckColor, stApiKey, stUsername, stStartup;
	Edit edApiKey, edUsername;
	UpDownEdit udeHours, udeMinutes, udeSeconds;
	Button btnOk, btnCancel, btnUpdate, btnDonate;
	CheckBox cbStartup;
	ColorPickerButton cpbBckColor;
};