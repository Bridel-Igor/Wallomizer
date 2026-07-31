#include "App.h"

#include <thread>

#include "TrayWindow.h"

App::App() :
	m_appMutex("Wallomizer"),
	m_delay(*this),
	m_collectionManager(*this),
	m_settings(m_winUtils)
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
		std::thread delayThread(&Delay::delay, &m_delay);
		m_collectionManager.loadNextWallpaper();
		delayThread.join();
		if (!m_running)
			break;
		m_collectionManager.setLoadedWallpaper();
	}

	trayWindowThread.join();
	if (trayWindowException)
		std::rethrow_exception(trayWindowException);
	return 0;
}

void App::exit()
{
	m_delay.saveSession(m_collectionManager.pCurrent);
	m_delay.abortDelay();
	m_running = false;
}
