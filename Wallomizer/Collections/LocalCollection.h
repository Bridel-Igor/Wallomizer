#pragma once

#include <filesystem>

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class CollectionManager;
class WinUtils;

class LocalCollection : public BaseCollection
{
private:
	struct LocalCollectionSettings
	{
		wchar_t wsDirectoryPath[255] = L"";
		bool bRecursive = false;
	};

public:
	LocalCollection(CollectionManager& collectionManager) :
		m_collectionManager(collectionManager)
	{}

	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile, unsigned short fileVersion);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	Collection::Type getCollectionType() const { return Collection::Type::local; }
	CategoriesAndPurity getCAP() const { return 0; }
	Wallpaper getWallpaper(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(std::filesystem::path sourcePath, const WinUtils& winUtils);
	static void openWallpaperExternal(std::filesystem::path sourcePath);

	LocalCollectionSettings settings;

private:
	CollectionManager& m_collectionManager;
};
