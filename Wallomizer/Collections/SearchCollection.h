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
	SearchCollection(const Settings& settings, CollectionManager& collectionManager) :
		BaseCollection(Collection::Type::search),
		m_settings(settings),
		m_collectionManager(collectionManager)
	{}
	~SearchCollection() = default;

	bool saveSettings(BinaryWriter& file) const override;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion) override;
	std::wstring getCollectionName() const override;
	CategoriesAndPurity getCAP() const override;
	Wallpaper getWallpaper(std::size_t index) const override;
	void update() override;
	bool isValid() const override;

	static bool loadWallpaper(std::wstring_view path, const WinUtils& winUtils);
	static void openWallpaperExternal(std::wstring_view path);

	SearchCollectionSettings settings;

private:
	std::wstring getURL() const;

	CollectionManager& m_collectionManager;
	const Settings& m_settings;

	static std::uint32_t s_perPage;
};
