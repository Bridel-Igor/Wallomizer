#pragma once

#include <list>
#include <mutex>

#include "Wallpaper.h"

class App;

class WallpaperManager
{
public:
	WallpaperManager(App& app);
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
	App& m_app;

	std::mutex m_imageModification;

	std::list<Wallpaper> m_wallpaperList;
	Wallpaper m_NextWallpaper;
};
