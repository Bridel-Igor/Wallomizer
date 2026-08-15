#include "App.h"

#include "TrayWindow.h"
#include "UIThreadedWindow.h"

App::App() :
	m_instanceGuard("Wallomizer"),
	m_winUtils(),
	m_settings(m_winUtils.getRoamingDir()),
	m_collectionManager(m_winUtils, m_settings, m_wallpaperManager, m_timer),
	m_wallpaperManager(m_winUtils, m_settings, m_collectionManager, m_timer),
	m_timer(m_winUtils, m_settings, m_wallpaperManager),
	m_ui(*this)
{
}

int App::run()
{
	while (m_running)
	{
		if (m_collectionManager.getWallpaperCount() == 0)
		{
			Sleep(100);
			continue;
		}

		std::thread loaderThread(&WallpaperManager::loadNextWallpaper, &m_wallpaperManager);
		m_timer.run();
		loaderThread.join();

		if (!m_running)
			break;

		m_wallpaperManager.setLoadedWallpaper();
	}

	m_ui.requestClose();
	return 0;
}

void App::requestExit()
{
	m_running = false;
	m_timer.abort();
	m_timer.saveSession();
}
