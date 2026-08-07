#pragma once

#include <string>
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
		std::uint32_t id;
		wchar_t wsLabel[64];
	};

private:
	struct UserCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity = CAP::puritySFW;
		std::wstring username = L"";
		std::wstring collectionID = L"";
		std::wstring collectionName = L"";
	};

public:
	UserCollection(App& app);

	~UserCollection() {};
	bool saveSettings(BinaryWriter& file) const;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion);
	std::wstring getCollectionName() const;
	CategoriesAndPurity getCAP() const;
	Wallpaper getWallpaper(std::size_t index) const;
	void openCollectionSettingsWindow(HWND hCaller);
	void update();

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);
	static void loadCollectionList(App& app, std::list<UserCollectionInfo>& list, const wchar_t* sUsername, const wchar_t* sApiKey);

	UserCollectionSettings settings;
	
private:
	std::wstring getURL() const;

	App& m_app;
	static constexpr std::uint32_t s_perPage = 24;
};
