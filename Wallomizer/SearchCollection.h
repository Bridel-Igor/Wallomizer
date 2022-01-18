#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class SearchCollection : public BaseCollection
{
private:
	struct SearchCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity;
		char tag[255];
		char resolution[255];
		char ratio[128];
		char color[16];
	};

public:
	SearchCollection();
	~SearchCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool getWallpaperInfo(Wallpaper*& wallpaper, unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	LPCSTR collectionType() const { return "Search collection"; }
	LPCSTR collectionName() const;
	CategoriesAndPurity getCAP();
	void openCollectionSettingsWindow();
	static void openWallpaperExternal(Wallpaper* wallpaper);

	SearchCollectionSettings* settings;

private:
	char searchUrl[1024] = "";
	int per_page = 24;
};