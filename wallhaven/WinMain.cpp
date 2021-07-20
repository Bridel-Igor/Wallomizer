#include <windows.h>
#include <thread>

#include "TrayWindow.h"
#include "CollectionManager.h"
#include "Settings.h"

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

	while (TrayWindow::trayWindow!=nullptr)
	{
		std::thread delayThread(Settings::Delay);
		CollectionManager::setNextWallpaper(); //maybe divide into loadRandomWallpaper & setWallpaper to even delay
		delayThread.join();
	}

	CollectionManager::clear();
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
	return 0;
}