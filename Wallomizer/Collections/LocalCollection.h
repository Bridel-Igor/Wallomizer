#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class App;

class LocalCollection : public BaseCollection
{
private:
	struct LocalCollectionSettings
	{
		wchar_t wsDirectoryPath[255] = L"";
		bool bRecursive = false;
	};

public:
	LocalCollection(App& app);

	~LocalCollection() {}
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile, unsigned short fileVersion);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	CollectionType getCollectionType() const { return CollectionType::local; }
	CategoriesAndPurity getCAP() const { return 0; }
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(const Wallpaper* pWallpaper, App& app);
	static void openWallpaperExternal(const Wallpaper* pWallpaper);

	LocalCollectionSettings settings;

private:
	App& m_app;
};