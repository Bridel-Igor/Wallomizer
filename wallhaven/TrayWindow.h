#pragma once

#include "BaseWindow.h"
#include <mutex>

class TrayWindow : public BaseWindow<TrayWindow>
{
public:
	LPCSTR ClassName() const { return "Tray Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(std::mutex *slideshow);
	
	std::mutex *slideshow;
	HMENU hPopup;

private:
	BOOL ShowPopupMenu(HWND hWnd, HINSTANCE hInstance, WORD nResourceID);

	static TrayWindow *trayWindow;
};