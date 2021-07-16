#include <windows.h>
#include <thread>
#include <mutex>
#include "time.h"

#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"

void Delay()
{
	unsigned int delayed = 0;
	while (delayed<Settings::delay)
	{
		if (!Settings::canRunDelay())
		{
			Settings::delayAborted();
			return;
		}
		Sleep(1000);
		delayed += 1000;
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "wallhaven");							//maybe change to IsProcessRunning
    if (GetLastError() == ERROR_ALREADY_EXISTS)										//
    {																				//
        MessageBoxA(nullptr, "Program is already launched!", "wallhaven", MB_OK);	//
        return 0;																	//
    }																				//

	std::mutex slideshow;
	std::thread thr(TrayWindow::windowThread, &slideshow);
	thr.detach();

	srand((unsigned int)time(NULL));
	Settings::loadSettings();
	CollectionManager::loadSettings();
	while (true)
	{
		std::thread delayThread(Delay);
		slideshow.lock();
		slideshow.unlock();
		if (!CollectionManager::setRandomWallpaper()) //maybe divide into loadRandomWallpaper & setWallpaper to even delay
			Settings::abortDelay();
		delayThread.join();
	}
	CollectionManager::clear();
	return 0;
}