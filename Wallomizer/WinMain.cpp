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
				WindowStyles::initialize();
				TrayWindow trayWindow(&collectionManager);
				trayWindow.windowLoop();
				WindowStyles::clear();
			}
			catch (...)
			{	
				WindowStyles::clear(); 
				trayWindowException = std::current_exception();
			}
		});
		
		while (true)
		{
			if (collectionManager.getNumber() == 0)
			{
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