#include "App.h"

#include <thread>

#include "TrayWindow.h"

App::App() :
	m_instanceGuard("Wallomizer"),
	m_settings(m_winUtils),
	m_collectionManager(*this),
	m_wallpaperManager(*this),
	m_timer(m_winUtils, m_settings, m_wallpaperManager)
{
}

int App::run()
{
	// opening and processing UI in different thread
	std::exception_ptr trayWindowException = nullptr;
	std::thread trayWindowThread([this, &trayWindowException]()
		{
			try
			{
				TrayWindow trayWindow(*this);
				trayWindow.windowLoop();
			}
			catch (...)
			{
				trayWindowException = std::current_exception();
			}
		});

	while (m_running)
	{
		if (m_collectionManager.getNumber() == 0)
		{
			if (!m_running)
				break;
			Sleep(100);
			continue;
		}
		std::thread timerThread(&Timer::run, &m_timer);
		m_wallpaperManager.loadNextWallpaper();
		timerThread.join();
		if (!m_running)
			break;
		m_wallpaperManager.setLoadedWallpaper();
	}

	trayWindowThread.join();
	if (trayWindowException)
		std::rethrow_exception(trayWindowException);
	return 0;
}

void App::requestExit()
{
	m_timer.saveSession();
	m_timer.abort();
	m_running = false;
}
