#pragma once

#include <string>

#include "IWindow.h"
#include "Static.h"
#include "PushButton.h"
#include "Button.h"

class AspRatPickerWindow : public IWindow
{
public:
	AspRatPickerWindow(IWindow* pOwner, std::wstring& aspRat);

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	Static stWide, stUltrawide, stPortrait, stSquare;
	PushButton btnAR[12], btnAllWide, btnAllPortrait;
	Button btnOk, btnCancel;	

	std::wstring& m_aspRat;
};
