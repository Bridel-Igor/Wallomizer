#pragma once

#include <list>
#include <vector>

#include "BaseCollection.h"

class CollectionManager : public BaseCollection
{
public:
	CollectionManager() = delete;
	CollectionManager(CollectionManager&) = delete;
	void operator=(const CollectionManager&) = delete;
	static bool saveSettings();
	static bool loadSettings();
	static void reloadSettings();
	static void updateNumber();
	template <typename T> static void addCollection();
	static void eraseCollection(int index);
	static void clear();
	static void loadRandomWallpaper();
	static bool getWallpaperInfo(Wallpaper*& wallpaper, int index);
	static bool loadWallpaper(Wallpaper *wallpaper);
	static void loadNextWallpaper();
	static void setLoadedWallpaper(bool setPrevious = false);
	static void setNextWallpaper();
	static void setPreviousWallpaper();
	static bool isPrevious();
	static LPCSTR collectionType() { return "Collection manager. You shouldn't see this"; }
	static unsigned int getNumber() { return number; }
	static void openCollectionSettingsWindow() { return; }
	static void openWallpaperExternal();
	static bool isReady() { return bIsReady; }

	static std::vector<BaseCollection*> collections;
	static bool bLoading;
	
private:
	static unsigned int number;
	static std::list<Wallpaper*> previous;
	static Wallpaper *current, *next;
	static bool bIsReady;
};