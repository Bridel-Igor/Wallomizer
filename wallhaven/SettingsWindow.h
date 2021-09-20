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
	Static* stHours, * stMinutes, * stSeconds, * stDelay, * stApiKey, * stUsername, *stStartup;
	Edit* edApiKey, * edUsername;
	UpDownEdit* udeHours, * udeMinutes, * udeSeconds;
	Button* btnOk, * btnCancel;
	CheckBox* cbStartup;
	char tmpRes[255];

public:
	LPCSTR ClassName() const { return "Settings Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();

	static constexpr int width = 350;
	static constexpr int height = 180;
	static SettingsWindow* settingsWindow;
};