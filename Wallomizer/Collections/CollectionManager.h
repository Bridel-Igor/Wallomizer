#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <random>

#include "BaseCollection.h"

class AppState;
class WinUtils;
class Settings;
class WallpaperManager;
class IUI;
class Wallpaper;

class CollectionManager
{
private:
	enum class LoadResult : std::uint8_t
	{
		success,
		failed,
		noFile,
		needsUpdate
	};

public:
	CollectionManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager, const IUI& ui);
	CollectionManager(const CollectionManager&) = delete;
	CollectionManager& operator=(const CollectionManager&) = delete;
	CollectionManager(CollectionManager&&) = delete;
	CollectionManager& operator=(CollectionManager&&) = delete;
	~CollectionManager() = default;

	bool saveSettings() const;
	LoadResult loadSettings();
	void recountWallpapers();
	void addCollection(std::unique_ptr<BaseCollection> collection);
	void eraseCollection(std::size_t index);
	void enableCollection(std::size_t index, bool enabled);

	Wallpaper getWallpaper(std::size_t index) const;
	Wallpaper getRandomWallpaper() const;
	
	std::size_t getWallpaperCount() const noexcept					{ return m_wallpaperCount; }
	BaseCollection& getCollection(std::size_t index)				{ return *m_collections.at(index); }
	const BaseCollection& getCollection(std::size_t index) const	{ return *m_collections.at(index); }
	std::size_t getCollectionCount() const noexcept					{ return m_collections.size(); }

private:
	static constexpr std::uint16_t FILE_VERSION = 4U;

	AppState& m_appState;
	const WinUtils& m_winUtils;
	const Settings& m_settings;
	WallpaperManager& m_wallpaperManager;
	const IUI& m_ui;

	std::vector<std::unique_ptr<BaseCollection>> m_collections;
	std::size_t m_wallpaperCount = 0;

	mutable std::minstd_rand m_randomGenerator;
	mutable std::size_t m_lastRandomIndex = std::numeric_limits<std::size_t>::max();
};
