#include "App.h"

#include <thread>

#include "TrayWindow.h"
#include "UIThreadedWindow.h"

App::App() :
	m_instanceGuard("Wallomizer"),
	m_appState(),
	m_winUtils(),
	m_settings(m_winUtils.getRoamingDir()),
	m_timer(m_settings),
	m_collectionManager(m_appState, m_winUtils, m_settings, m_wallpaperManager, m_timer),
	m_wallpaperManager(m_appState, m_winUtils, m_settings, m_collectionManager, m_timer),
	m_ui(*this)
{
}

int App::run()
{
	while (!m_appState.isExiting())
	{
		if (m_appState.isNoWallpapers() || m_timer.isStopped())
		{
			Player::redrawPlayers();
			Sleep(100);
			continue;
		}
		m_appState.setRunning();

		std::thread loaderThread(&WallpaperManager::loadImage, &m_wallpaperManager);
		m_timer.run();
		
		m_appState.setLoading(true);
		Player::redrawPlayers();

		loaderThread.join();

		m_appState.setLoading(false);
		Player::redrawPlayers();
		
		if (m_appState.isExiting() || m_timer.isStopped())
			continue;

		m_wallpaperManager.applyLoadedImage();
	}

	m_ui.requestQuit();
	return 0;
}

void App::requestExit()
{
	m_appState.setExiting();
	m_wallpaperManager.saveSession();
	m_timer.cancel();
}
