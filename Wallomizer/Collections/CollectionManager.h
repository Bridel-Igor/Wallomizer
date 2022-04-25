#pragma once

#include <list>
#include <vector>
#include <random>

#include "BaseCollection.h"

class CollectionManager : public BaseCollection
{
public:
	CollectionManager();
	~CollectionManager();
	CollectionManager(CollectionManager&) = delete;
	void operator=(const CollectionManager&) = delete;
	bool saveSettings(FILE* pFile = nullptr);
	bool loadSettings(FILE* pFile = nullptr);
	void reloadSettings();
	void updateNumber();
	template <typename T> void addCollection();
	void eraseCollection(int index);
	void clear();
	void loadRandomWallpaper();
	Wallpaper* getWallpaperInfo(unsigned int index);
	static bool loadWallpaper(Wallpaper *wallpaper);
	void loadNextWallpaper();
	void setLoadedWallpaper(bool setPrevious = false);
	void setNextWallpaper();
	void setPreviousWallpaper();
	bool isPrevious();
	LPCSTR collectionType() const { return "Collection manager. You shouldn't see this"; }
	LPCSTR collectionName() const { return ""; };
	CategoriesAndPurity getCAP() { return 0; };
	unsigned int getNumber() { return number; }
	void openCollectionSettingsWindow() { return; }
	void openWallpaperExternal();
	bool isReady() { return bIsReady; }

	std::vector<BaseCollection*> collections;
	bool bLoading;
	Wallpaper *current, *next;
	
private:
	std::list<Wallpaper*> previous;
	bool bIsReady;
	std::mt19937 rndGen;
	std::uniform_int_distribution<int> uid;
};