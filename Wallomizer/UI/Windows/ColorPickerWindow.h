#pragma once

#include <string>

#include "IWindow.h"
#include "ColorButton.h"
#include "Button.h"

class ColorPickerWindow : public IWindow
{
public:
	ColorPickerWindow(IWindow* pOwner, std::wstring& color);

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	ColorButton btnClr[30];
	Button btnOk, btnCancel;
	
	std::wstring& m_color;
};
