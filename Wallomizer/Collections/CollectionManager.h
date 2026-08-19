#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <random>

class AppState;
class WinUtils;
class Settings;
class WallpaperManager;
class Timer;
class Wallpaper;
class BaseCollection;

class CollectionManager
{
public:
	CollectionManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager, Timer& timer);
	CollectionManager(const CollectionManager&) = delete;
	CollectionManager& operator=(const CollectionManager&) = delete;
	CollectionManager(CollectionManager&&) = delete;
	CollectionManager& operator=(CollectionManager&&) = delete;
	~CollectionManager();

	bool saveSettings() const;
	bool loadSettings();
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
	Timer& m_timer;

	std::vector<std::unique_ptr<BaseCollection>> m_collections;
	std::size_t m_wallpaperCount = 0;
	mutable std::mt19937 m_randomGenerator;
};
