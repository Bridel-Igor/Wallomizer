#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"
#include "CollectionManager.h"

class LocalCollection : public BaseCollection
{
public:
	LocalCollection(CollectionManager* collectionManager);
	~LocalCollection() {}
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	Wallpaper* getWallpaperInfo(unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	LPCSTR collectionType() const { return "Local collection"; };
	LPCWSTR collectionName() const;
	CollectionType getCollectionType() const { return CollectionType::local; };
	CategoriesAndPurity getCAP();
	void openCollectionSettingsWindow();
	static void openWallpaperExternal(Wallpaper* wallpaper);

	wchar_t directoryPath[255];

private:
	CollectionManager* m_pCollectionManager = nullptr;
};