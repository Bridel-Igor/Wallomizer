#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "ColorButton.h"
#include "SearchCollection.h"

class ColorPickerWindow : public BaseWindow<ColorPickerWindow>
{
	ColorButton *btnClr[30];
	Button* btnOk, * btnCancel;
	static ColorPickerWindow* colorPickerWindow;
	static char* color;

public:
	LPCSTR ClassName() const { return "Color Picker Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(char* _color, HWND caller);
};