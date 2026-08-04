#pragma once

#include <string_view>

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"

class App;
class WinUtils;

class UserCollection : public BaseCollection
{
public:
	struct UserCollectionInfo
	{
		unsigned int id;
		wchar_t wsLabel[64];
	};

private:
	struct UserCollectionSettings
	{
		wchar_t wsUsername[64] = L"";
		wchar_t wsCollectionID[16] = L"";
		wchar_t wsCollectionName[64] = L"";
		CategoriesAndPurity categoriesAndPurity = CAP::puritySFW;
	};

public:
	UserCollection(App& app);

	~UserCollection() {};
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile, unsigned short fileVersion);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	Collection::Type getCollectionType() const { return Collection::Type::user; }
	CategoriesAndPurity getCAP() const;
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);
	static void loadCollectionList(App& app, std::list<UserCollectionInfo>& list, const wchar_t* sUsername, const wchar_t* sApiKey);

	UserCollectionSettings settings;
	
private:
	App& m_app;
	static constexpr int s_nPerPage = 24;
	wchar_t m_wsCollectionUrl[255] = L"";
};
