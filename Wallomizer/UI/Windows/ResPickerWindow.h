#pragma once

#include "IWindow.h"
#include "Button.h"
#include "PushButton.h"
#include "Static.h"
#include "Edit.h"

class ResPickerWindow : public IWindow
{
public:
	ResPickerWindow(HWND hCaller, wchar_t *sResolution);
	~ResPickerWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	wchar_t* m_wsResolution;
	Static stUltrawide, st169, st1610, st43, st54, stCustom, stX;
	PushButton btnRes[23], btnAtLeast, btnExactly;
	Button btnOk, btnCancel;
	Edit edWidth, edHeight;
};