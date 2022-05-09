#pragma once

#include "IWindow.h"
#include "Button.h"
#include "ColorButton.h"
#include "SearchCollection.h"

class ColorPickerWindow : public IWindow
{
public:
	ColorPickerWindow(HWND hCaller, wchar_t* wsColor);
	~ColorPickerWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	wchar_t* m_sColor;
	ColorButton btnClr[30];
	Button btnOk, btnCancel;
};