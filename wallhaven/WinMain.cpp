#include <windows.h>
#include "time.h"

#include "Settings.h"
#include "UserCollection.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	srand((unsigned int)time(NULL));
	Settings *settings = new Settings;
	UserCollection *collection = new UserCollection(settings);
	while (true)
	{
		if (!collection->setRandomWallpaper())
		{
			MessageBoxA(nullptr, "Can't set wallpaper. Exiting...", "wallhaven", MB_OK);
			break;
		}
		Sleep(settings->delay);
	}
	delete collection;
	delete settings;
	return 0;
}