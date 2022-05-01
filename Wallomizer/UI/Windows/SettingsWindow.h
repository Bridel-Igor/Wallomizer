#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CheckBox.h"

class SettingsWindow : public IWindow
{
public:
	SettingsWindow();
	~SettingsWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 400;
	static constexpr int height = 310;

private:
	Static stApplication, stSlideshow, stWallhaven;
	Static stVersion, stActVersion, stHours, stMinutes, stSeconds, stDelay, stApiKey, stUsername, stStartup;
	Edit edApiKey, edUsername;
	UpDownEdit udeHours, udeMinutes, udeSeconds;
	Button btnOk, btnCancel, btnUpdate;
	CheckBox cbStartup;
};