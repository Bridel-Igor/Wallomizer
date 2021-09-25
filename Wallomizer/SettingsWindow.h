#pragma once

#include "BaseWindow.h"
#include "Static.h"
#include "Edit.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CheckBox.h"

class SettingsWindow : public BaseWindow<SettingsWindow>
{
private:
	Static* stApplication, * stSlideshow, * stWallhaven;
	Static* stVersion, *stActVersion, * stHours, * stMinutes, * stSeconds, * stDelay, * stApiKey, * stUsername, *stStartup;
	Edit* edApiKey, * edUsername;
	UpDownEdit* udeHours, * udeMinutes, * udeSeconds;
	Button* btnOk, * btnCancel, * btnUpdate;
	CheckBox* cbStartup;
	char tmpRes[255];

public:
	LPCSTR ClassName() const { return "Settings Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();

	static constexpr int width = 400;
	static constexpr int height = 310;
	static SettingsWindow* settingsWindow;
};