#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"
#include "CollectionManager.h"

class LocalCollection : public BaseCollection
{
public:
	LocalCollection(CollectionManager* pCollectionManager) : 
		m_pCollectionManager(pCollectionManager) 
	{}

	~LocalCollection() {}
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	CollectionType getCollectionType() const { return CollectionType::local; }
	CategoriesAndPurity getCAP() const { return 0; }
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(const Wallpaper* pWallpaper);
	static void openWallpaperExternal(const Wallpaper* pWallpaper);

	wchar_t m_wsDirectoryPath[255] = L"";

private:
	CollectionManager* m_pCollectionManager = nullptr;
};

// TODO: recursive selection of directories