#include "UI.h"

#include "App.h"
#include "UIMessage.h"

UI::UI(App& app) :
	m_app(app)
{
	m_trayIcon.start(std::ref(m_app.getUI()));
	m_trayIcon.waitUntilReady();
	if (m_app.getAppState().isFirstLaunch())
		openMainWindowAsync();
}

void UI::openTrayWindowAsync(POINT pt)
{
	m_trayWindow.start(std::ref(m_app), pt);
}

void UI::openMainWindowAsync()
{
	m_mainWindow.start(std::ref(m_app));
}

void UI::requestQuit()
{
	m_mainWindow.requestQuit();
	m_trayWindow.requestQuit();
	m_trayIcon.requestQuit();
}

void UI::queryEndSession()
{
	m_app.getWallpaperManager().saveSession();
}

void UI::requestCollectionListUpdate() const
{
	const HWND hwndMain = m_mainWindow.getHandle();

	if (hwndMain)
		PostMessageW(hwndMain, UIMessage::REFRESH_TABLE, 0, 0);
}

void UI::requestPlayerUpdate(bool timerOnly) const
{
	const WPARAM wParam = timerOnly ? 1 : 0;

	const HWND hwndMain = m_mainWindow.getHandle();
	if (hwndMain)
		PostMessageW(hwndMain, UIMessage::REFRESH_PLAYER, wParam, 0);

	const HWND hwndTray = m_trayWindow.getHandle();
	if (hwndTray)
		PostMessageW(hwndTray, UIMessage::REFRESH_PLAYER, wParam, 0);
}
