#pragma once

#include <list>

#include "IWindow.h"
#include "Button.h"
#include "Player.h"

class App;

class TrayWindow : public IWindow
{
public:
	TrayWindow(App& app);
	~TrayWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	static constexpr int width = 220, height = 90;
	static TrayWindow *s_pTrayWindow;

private:
	App& m_app;

public:
	Button btnSettings, btnExit;

private:
	Player player;
	HICON hStatusIcon = nullptr;
	LPCSTR pszIDStatusIcon = nullptr;
};