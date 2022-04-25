#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"
#include "CollectionManager.h"

class LocalCollection : public BaseCollection
{
public:
	LocalCollection(CollectionManager* collectionManager);
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool getWallpaperInfo(Wallpaper*& wallpaper, unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	LPCSTR collectionType() const { return "Local collection"; };
	LPCSTR collectionName() const;
	CategoriesAndPurity getCAP();
	void openCollectionSettingsWindow();
	static void openWallpaperExternal(Wallpaper* wallpaper);

	char directoryPath[255];

private:
	CollectionManager* m_pCollectionManager = nullptr;
};