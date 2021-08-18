#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "PushButton.h"
#include "Static.h"
#include "Edit.h"

class ResPickerWindow : public BaseWindow<ResPickerWindow>
{
	Static* stUltrawide, * st169, * st1610, * st43, * st54, *stCustom, *stX;
	PushButton* btnRes[23], * btnAtLeast, * btnExactly;
	Button *btnOk, *btnCancel;
	Edit* edWidth, * edHeight;
	static ResPickerWindow* resPickerWindow;
	static char* res;
	HFONT font;
	HBRUSH bkBrush;

public:
	LPCSTR ClassName() const { return "Resolution Picker Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(char *res, HWND caller);
};