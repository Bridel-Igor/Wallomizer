#include <thread>

#include "AppMutex.h"
#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"
#include "WindowStyles.h"
#include "Filesystem.h"
#include "Delay.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    try
	{
		AppMutex appMutex("Wallomizer");
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

		Filesystem::initialize();
		WindowStyles::initialize();

		std::thread thr(TrayWindow::windowThread);
		thr.detach();

		unsigned short waitedForTrayWindow = 0;
		while (!TrayWindow::isReady())
		{
			if (waitedForTrayWindow >= 5000)
			{
				WindowStyles::clear();
				throw std::exception("Wallomizer was unable to start.");
			}
			Sleep(10);
			waitedForTrayWindow += 10;
		}

		Settings::loadSettings();
		CollectionManager::loadSettings();
		Delay::loadSession(CollectionManager::current);

		while (TrayWindow::isReady())
		{
			if (CollectionManager::getNumber() == 0)
			{
				Sleep(100);
				continue;
			}
			std::thread delayThread(Delay::Delay);
			CollectionManager::loadNextWallpaper();
			delayThread.join();
			if (Delay::exiting)
				break;
			CollectionManager::setLoadedWallpaper();
		}

		WindowStyles::clear();
		CollectionManager::clear();
		return 0;
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}