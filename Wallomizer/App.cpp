#include "App.h"

#include <thread>

App::App() :
	m_instanceGuard("Wallomizer"),
	m_appState(),
	m_winUtils(),
	m_settings(m_winUtils.getRoamingDir()),
	m_timer(m_settings),
	m_ui(*this),
	m_collectionManager(m_appState, m_winUtils, m_settings, m_wallpaperManager, m_ui),
	m_wallpaperManager(m_appState, m_winUtils, m_settings, m_collectionManager, m_timer, m_ui)
{
}

int App::run()
{
	while (!m_appState.isExiting())
	{
		if (m_appState.isNoWallpapers() 
			|| m_appState.isFileError() 
			|| m_appState.isNoInternet() 
			|| m_timer.isStopped())
		{
			m_ui.requestPlayerUpdate();
			Sleep(100);
			continue;
		}
		m_appState.setRunning();

		std::thread loaderThread(&WallpaperManager::loadImage, &m_wallpaperManager);
		m_timer.run(m_ui);
		
		m_appState.setLoading(true);
		m_ui.requestPlayerUpdate();

		loaderThread.join();

		m_appState.setLoading(false);
		m_ui.requestPlayerUpdate();
		
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
