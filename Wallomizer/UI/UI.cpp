#include "UI.h"

#include "App.h"

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
