#pragma once

#include "IWindow.h"
#include "Button.h"
#include "Player.h"

class App;

/// Temporary popup window displayed near the system-tray icon.
///
/// The window is created on demand when the tray icon is activated and is
/// destroyed when it loses focus or when one of its actions requires it to close.
class TrayWindow : public IWindow
{
public:
	/// Creates the tray popup at the specified screen position.
	TrayWindow(App& app, POINT point);
	
	~TrayWindow() = default;
	
private:
	/// Handles window activation, owner-draw messages and control commands.
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	/// Positions the popup around the tray icon while keeping it inside
	/// the work area of the monitor containing the icon.
	void positionWindow(POINT point);

	static constexpr int width = 220, height = 90;

	App& m_app;

	Button btnSettings, btnExit;
	Player player;
};
