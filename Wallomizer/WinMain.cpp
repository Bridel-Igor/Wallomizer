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
		CollectionManager collectionManager;

		std::exception_ptr trayWindowThreadException = nullptr;
		std::thread trayWindowThread(TrayWindow::windowThread, std::ref(trayWindowThreadException), &collectionManager);
		unsigned short waitedForTrayWindow = 0;
		while (!TrayWindow::isReady())
		{
			if (waitedForTrayWindow >= 5000)
				throw std::exception("Wallomizer was unable to start.");
			Sleep(10);
			waitedForTrayWindow += 10;
		}

		Settings::loadSettings();
		collectionManager.loadSettings();
		Delay::loadSession(collectionManager.current);

		while (TrayWindow::isReady())
		{
			if (collectionManager.getNumber() == 0)
			{
				Sleep(100);
				continue;
			}
			std::thread delayThread(Delay::Delay);
			collectionManager.loadNextWallpaper();
			delayThread.join();
			if (Delay::exiting)
				break;
			collectionManager.setLoadedWallpaper();
		}

		trayWindowThread.join();
		if (trayWindowThreadException)
			std::rethrow_exception(trayWindowThreadException);
		WindowStyles::clear();
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
	WindowStyles::clear();
	return -1;
}