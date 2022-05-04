#include <thread>

#include "AppMutex.h"
#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"
#include "Filesystem.h"
#include "Delay.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    try
	{
		AppMutex appMutex("Wallomizer"); // making shure application is opened in only one instance
		SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

		Filesystem::initialize(); // checking for all necessary directories
		Settings::loadSettings();
		CollectionManager collectionManager;
		Delay::loadSession(collectionManager.pCurrent);

		// opening and processing UI in different thread
		std::exception_ptr trayWindowException = nullptr;
		std::thread trayWindowThread([&collectionManager, &trayWindowException]()
		{
			try
			{
				TrayWindow trayWindow(&collectionManager);
				trayWindow.windowLoop();
			}
			catch (...)
			{
				trayWindowException = std::current_exception();
			}
		});
		
		while (true)
		{
			if (collectionManager.getNumber() == 0)
			{
				if (Delay::exiting)
					break;
				Sleep(100);
				continue;
			}
			std::thread delayThread(Delay::delay);
			collectionManager.loadNextWallpaper();
			delayThread.join();
			if (Delay::exiting)
				break;
			collectionManager.setLoadedWallpaper();
		}

		trayWindowThread.join();
		if (trayWindowException)
			std::rethrow_exception(trayWindowException);
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