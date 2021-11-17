#pragma once

#include "BaseCollection.h"

class LocalCollection : public BaseCollection
{
public:
	LocalCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool getWallpaperInfo(Wallpaper*& wallpaper, unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	LPCSTR collectionType() const { return "Local collection"; };
	LPCSTR collectionName() const;
	void openCollectionSettingsWindow();
	static void openWallpaperExternal(Wallpaper* wallpaper);

	char directoryPath[255];
};