#pragma once

#include <string>
#include <string_view>

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"

class WinUtils;
class Settings;

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
	UserCollection(const Settings& settings);

	~UserCollection() {};
	bool saveSettings(BinaryWriter& file) const;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion);
	std::wstring getCollectionName() const;
	CategoriesAndPurity getCAP() const;
	Wallpaper getWallpaper(std::size_t index) const;
	void update();
	bool validate() const;

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);
	static void loadCollectionList(std::list<UserCollectionInfo>& list, const std::wstring& username, const std::wstring& apiKey);

	UserCollectionSettings settings;
	
private:
	std::wstring getURL() const;

	const Settings& m_settings;
	static constexpr std::uint32_t s_perPage = 24;
};
