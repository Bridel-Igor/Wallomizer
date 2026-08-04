#pragma once

#include <string_view>

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
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);

	LocalCollectionSettings settings;

private:
	CollectionManager& m_collectionManager;
};
