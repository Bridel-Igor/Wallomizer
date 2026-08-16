#pragma once

#include "UIThreadedWindow.h"
#include "TrayIcon.h"
#include "TrayWindow.h"
#include "MainWindow.h"

class App;

/// Coordinates the lifetime and interaction of application UI windows.
/// 
/// The tray icon is created when the UI is initialized and remains alive for
/// the entire application lifetime. Other windows are created on demand and
/// run in their own UI threads.
class UI
{
public:
	/// Initializes the UI and starts the tray icon.
	/// The constructor does not return until the tray icon has been
	/// successfully created and its UI thread is ready.
	explicit UI(App& app);

	/// Opens the window asynchronously.
	/// If the window is already running, it is brought to the foreground instead of creating a duplicate.
	void openTrayWindowAsync(POINT pt);
	void openMainWindowAsync();

	/// Requests all UI windows to close and waits for their UI threads to finish.
	void requestQuit();

	/// Handles the Windows session-end notification.
	void queryEndSession();

private:
	App& m_app;

	UIThreadedWindow<TrayIcon> m_trayIcon;
	UIThreadedWindow<TrayWindow> m_trayWindow;
	UIThreadedWindow<MainWindow> m_mainWindow;
};
