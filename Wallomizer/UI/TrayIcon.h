#pragma once

#include "IWindow.h"

class UI;

/// Persistent system-tray icon.
///
/// The tray icon lives for the entire application lifetime and acts as the
/// entry point for opening other UI windows. It also receives the Windows
/// session-end notification and forwards it to the UI coordinator.
class TrayIcon : public IWindow
{
public:
	/// Creates and registers the application icon in the system tray.
	explicit TrayIcon(UI& ui);
	
	/// Removes the tray icon and releases its icon resource.
	~TrayIcon();

private:
	/// Handles tray notifications and Windows session-end messages.
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM, LPARAM lParam) override;

	/// Adds or removes the icon from the system tray.
	BOOL TrayMessage(DWORD dwMessage) const;

	/// Returns the screen-space center of the tray icon.
	POINT getPosition() const;

	static constexpr UINT WM_NOTIFYICONMSG = WM_USER + 2;
	static constexpr UINT id = 1;

	UI& m_ui;

	HICON m_hIcon = nullptr;
};
