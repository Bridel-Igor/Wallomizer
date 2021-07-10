#include <windows.h>
#include "time.h"

#include "UserCollection.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{    
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "wallhaven");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(nullptr, "Program is already launched. Exiting...", "wallhaven", MB_OK);
        return 0;
    }

	srand((unsigned int)time(NULL));
	UserCollection *collection = new UserCollection();
	while (true)
	{
		if (!collection->setRandomWallpaper())
		{
			MessageBoxA(nullptr, "Can't set wallpaper. Exiting...", "wallhaven", MB_OK);
			break;
		}
		Sleep(UserCollectionSettings::delay);
	}
	delete collection;
	return 0;
}