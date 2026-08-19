#pragma once

#include <filesystem>

#include "BaseCollection.h"

class CollectionManager;
class WinUtils;

class LocalCollection : public BaseCollection
{
public:
	LocalCollection(CollectionManager& collectionManager) :
		BaseCollection(CollectionType::local),
		m_collectionManager(collectionManager)
	{}
	~LocalCollection() = default;

	bool saveSettings(BinaryWriter& file) const override;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion) override;
	std::wstring getCollectionName() const override;
	CategoriesAndPurity getCAP() const noexcept override { return 0; }
	Wallpaper getWallpaper(std::size_t index) override;
	void update() override;
	bool isValid() const override;

	static bool loadWallpaper(std::filesystem::path sourcePath, const WinUtils& winUtils);
	static void openWallpaperExternal(std::filesystem::path sourcePath, const WinUtils& winUtils);

	const std::filesystem::path& getPath() const noexcept { return m_path; }
	void setPath(const std::filesystem::path& newPath) { m_path = newPath; }
	bool isRecursive() const noexcept { return m_isRecursive; }
	void setRecursive(bool value) noexcept { m_isRecursive = value; }

private:
	CollectionManager& m_collectionManager;
	
	std::filesystem::path m_path;
	std::uint8_t m_isRecursive = false;
};
