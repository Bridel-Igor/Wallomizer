#pragma once

#include "IWindow.h"
#include "Button.h"
#include "PushButton.h"
#include "Static.h"
#include "SearchCollection.h"

class AspRatPickerWindow : public IWindow
{
public:
	AspRatPickerWindow(char* sAspRat, HWND hCaller);
	~AspRatPickerWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	Static stWide, stUltrawide, stPortrait, stSquare;
	PushButton btnAR[12], btnAllWide, btnAllPortrait;
	Button btnOk, btnCancel;

	static AspRatPickerWindow* aspRatPickerWindow;
	char* m_sAspRat;
	HWND m_hCaller;
};