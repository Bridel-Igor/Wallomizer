#pragma once

#include "IWindow.h"
#include "Button.h"
#include "ColorButton.h"
#include "SearchCollection.h"

class ColorPickerWindow : public IWindow
{
public:
	ColorPickerWindow(char* sColor, HWND hCaller);
	~ColorPickerWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	ColorButton btnClr[30];
	Button btnOk, btnCancel;
	static ColorPickerWindow* colorPickerWindow;
	char* m_sColor;
	HWND m_hCaller;
};