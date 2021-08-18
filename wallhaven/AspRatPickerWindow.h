#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "PushButton.h"
#include "Static.h"
#include "SearchCollection.h"

class AspRatPickerWindow : public BaseWindow<AspRatPickerWindow>
{
	Static* stWide, * stUltrawide, * stPortrait, * stSquare;
	PushButton* btnAR[12], * btnAllWide, * btnAllPortrait;
	Button* btnOk, * btnCancel;
	static AspRatPickerWindow* aspRatPickerWindow;
	static char* aspRat;
	HFONT font;
	HBRUSH bkBrush;

public:
	LPCSTR ClassName() const { return "Aspect ratio Picker Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(char* _aspRat, HWND caller);
};