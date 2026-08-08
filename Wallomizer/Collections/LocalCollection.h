#pragma once

#include <filesystem>

#include "BaseCollection.h"

class CollectionManager;
class WinUtils;

class LocalCollection : public BaseCollection
{
public:
	LocalCollection(CollectionManager& collectionManager);

	bool saveSettings(BinaryWriter& file) const;
	bool loadSettings(BinaryReader& file, std::uint16_t fileVersion);
	std::wstring getCollectionName() const;
	CategoriesAndPurity getCAP() const noexcept { return 0; }
	Wallpaper getWallpaper(std::size_t index) const;

	static bool loadWallpaper(std::filesystem::path sourcePath, const WinUtils& winUtils);
	static void openWallpaperExternal(std::filesystem::path sourcePath);

	const std::filesystem::path& getPath() const noexcept { return m_path; }
	void setPath(const std::filesystem::path& newPath) { m_path = newPath; }
	bool isRecursive() const noexcept { return m_isRecursive; }
	void setRecursive(bool value) noexcept { m_isRecursive = value; }
	void update();
	bool isValid() const;

private:
	CollectionManager& m_collectionManager;
	
	std::filesystem::path m_path;
	std::uint8_t m_isRecursive = false;
};
