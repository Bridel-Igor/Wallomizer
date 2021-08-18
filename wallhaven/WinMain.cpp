#include <windows.h>
#include <thread>

#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"
#include "WindowStyles.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "wallhaven");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(nullptr, "Program is already launched!", "wallhaven", MB_OK);
        return 0;
    }
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	std::thread thr(TrayWindow::windowThread);
	thr.detach();
	Settings::loadSettings();
	CollectionManager::loadSettings();
	WindowStyles::initialize();

	unsigned int waitedForTrayWindow = 0;
	while (TrayWindow::trayWindow == nullptr)
	{
		if (waitedForTrayWindow >= 5000)
			break;
		Sleep(10);
		waitedForTrayWindow += 10;
	}

	while (TrayWindow::trayWindow!=nullptr)
	{
		if (CollectionManager::getNumber() == 0)
		{
			Sleep(100);
			continue;
		}
		std::thread delayThread(Settings::Delay);
		CollectionManager::loadNextWallpaper();
		delayThread.join();
		if (Settings::exiting)
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