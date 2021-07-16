#pragma once

#include <vector>
#include <string>

#include "BaseCollection.h"
#include "UserCollection.h"
#include "DirectoryCollection.h"

class CollectionManager : public BaseCollection
{
public:
	CollectionManager() = delete;
	CollectionManager(CollectionManager&) = delete;
	void operator=(const CollectionManager&) = delete;
	static bool saveSettings();
	static bool loadSettings();
	static void reloadSettings();
	template <typename T>
	static void addCollection()
	{
		BaseCollection *col =  new T;
		collections.push_back(col);
		col->openCollectionSettingsWindow();
		if (strcmp(collections.back()->collectionName(), "") == 0)
			collections.pop_back();
	}
	static void eraseCollection(int index);
	static void clear();
	static bool setRandomWallpaper();
	static LPCSTR collectionType() { return "Collection manager. You shouldn't see this"; }
	static unsigned int getNumber() { return number; }
	static void openCollectionSettingsWindow() { return; }
	//void setPreviousWallpaper();
	//void openWallpaperLocation();

	static std::vector<BaseCollection*> collections;

private:
	static unsigned int number;
};