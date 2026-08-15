#include "UI.h"

#include "App.h"

UI::UI(App& app) :
	m_app(app)
{
	m_trayIcon.start(std::ref(m_app.getUI()));
	m_trayIcon.waitUntilReady();
}

void UI::openTrayWindowAsync(POINT pt)
{
	m_trayWindow.start(std::ref(m_app), pt);
}

void UI::openMainWindowAsync()
{
	m_mainWindow.start(std::ref(m_app));
}

void UI::requestClose()
{
	m_mainWindow.requestClose();
	m_trayWindow.requestClose();
	m_trayIcon.requestClose();
}

void UI::queryEndSession()
{
	m_app.getTimer().saveSession();
}
