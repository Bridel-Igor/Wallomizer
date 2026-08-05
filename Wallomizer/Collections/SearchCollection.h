#pragma once

#include <string_view>

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"

class App;
class WinUtils;

class SearchCollection : public BaseCollection
{
private:
	struct SearchCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity = CAP::categoryGeneral | CAP::categoryAnime | CAP::categoryPeople | CAP::puritySFW;
		wchar_t wsTag[255] = L"";
		wchar_t wsResolution[255] = L"";
		wchar_t wsRatio[128] = L"";
		wchar_t wsColor[16] = L"";
	};

public:
	SearchCollection(App& app);

	~SearchCollection() {};
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile, unsigned short fileVersion);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	Collection::Type getCollectionType() const { return Collection::Type::search; }
	CategoriesAndPurity getCAP() const;
	Wallpaper getWallpaper(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);

	SearchCollectionSettings settings;

private:
	App& m_app;
	wchar_t m_wsSearchUrl[1024] = L"";	
};
