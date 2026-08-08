#pragma once

#include <list>
#include <mutex>

#include "Wallpaper.h"

class WinUtils;
class Settings;
class CollectionManager;
class Timer;

class WallpaperManager
{
public:
	WallpaperManager(const WinUtils& winUtils, const Settings& settings, const CollectionManager& collectionManager, Timer& timer);
	WallpaperManager(const WallpaperManager&) = delete;
	WallpaperManager& operator=(const WallpaperManager&) = delete;
	WallpaperManager(WallpaperManager&&) = delete;
	WallpaperManager& operator=(WallpaperManager&&) = delete;

	void setCurrentWallpaper(Wallpaper&& wallpaper);
	const Wallpaper getCurrentWallpaper() const;
	bool hasPrevious() const;
	void loadNextWallpaper();
	void setLoadedWallpaper(bool setPrevious = false);
	void setNextWallpaper();
	void setPreviousWallpaper();
	void deleteLoaded();
	void openCurrentWallpaperExternally();

private:
	const WinUtils& m_winUtils;
	const Settings& m_settings;
	const CollectionManager& m_collectionManager;
	Timer& m_timer;

	std::mutex m_imageModification;

	std::list<Wallpaper> m_wallpaperList;
	Wallpaper m_NextWallpaper;
};
