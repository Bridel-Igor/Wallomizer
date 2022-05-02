#pragma once

#include "IWindow.h"
#include "Button.h"
#include "PushButton.h"
#include "Static.h"
#include "SearchCollection.h"

class AspRatPickerWindow : public IWindow
{
public:
	AspRatPickerWindow(HWND hCaller, char* sAspRat);
	~AspRatPickerWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	char* m_sAspRat;
	Static stWide, stUltrawide, stPortrait, stSquare;
	PushButton btnAR[12], btnAllWide, btnAllPortrait;
	Button btnOk, btnCancel;	
};