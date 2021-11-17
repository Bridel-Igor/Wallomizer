#include <windows.h>
#include <thread>

#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"
#include "WindowStyles.h"
#include "Filesystem.h"
#include "Delay.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "Wallomizer");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(nullptr, "Wallomizer is already launched! You can find it in the system tray.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	Filesystem::initialize();
	WindowStyles::initialize();

	std::thread thr(TrayWindow::windowThread);
	thr.detach();
	
	unsigned int waitedForTrayWindow = 0;
	while (TrayWindow::trayWindow == nullptr)
	{
		if (waitedForTrayWindow >= 5000)
			break;
		Sleep(10);
		waitedForTrayWindow += 10;
	}
	
	Settings::loadSettings();
	CollectionManager::loadSettings();
	Delay::loadSession(CollectionManager::current);

	while (TrayWindow::trayWindow!=nullptr)
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
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
	return 0;
}