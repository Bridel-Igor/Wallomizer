#pragma once

#include "BaseWindow.h"

class TrayWindow : public BaseWindow<TrayWindow>
{
public:
	LPCSTR ClassName() const { return "Tray Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();
	
	static TrayWindow *trayWindow;

private:
	HMENU hPopup = nullptr, hMenu = nullptr;
	HICON hStatusIcon = nullptr;
	LPCSTR pszIDStatusIcon = nullptr;
};