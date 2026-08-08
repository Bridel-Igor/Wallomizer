#pragma once

#include <string>
#include <string_view>

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"

class CollectionManager;
class Settings;
class WinUtils;

class SearchCollection : public BaseCollection
{
private:
	struct SearchCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity = CAP::categoryGeneral | CAP::categoryAnime | CAP::categoryPeople | CAP::puritySFW;
		std::wstring tag = L"";
		std::wstring resolution = L"";
		std::wstring ratio = L"";
		std::wstring color = L"";
	};

public:
	SearchCollection(const Settings& settings, CollectionManager& collectionManager);
	~SearchCollection() {};

	bool saveSettings(BinaryWriter& file) const;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion);
	std::wstring getCollectionName() const;
	CategoriesAndPurity getCAP() const;
	Wallpaper getWallpaper(std::size_t index) const;
	void update();
	bool validate() const;

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);

	SearchCollectionSettings settings;

private:
	std::wstring getURL() const;

	CollectionManager& m_collectionManager;
	const Settings& m_settings;

	static std::uint32_t s_perPage;
};
