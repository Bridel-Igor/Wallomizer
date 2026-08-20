#pragma once

#include <list>
#include <mutex>
#include <filesystem>

#include "Wallpaper.h"

class AppState;
class WinUtils;
class Settings;
class CollectionManager;
class Timer;

class WallpaperManager
{
public:
	WallpaperManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, const CollectionManager& collectionManager, Timer& timer);
	WallpaperManager(const WallpaperManager&) = delete;
	WallpaperManager& operator=(const WallpaperManager&) = delete;
	WallpaperManager(WallpaperManager&&) = delete;
	WallpaperManager& operator=(WallpaperManager&&) = delete;

	bool saveSession();
	bool loadSession();

	// Player functions
	void previousWallpaper();
	void openCurrentWallpaperExternally();
	void stop();
	void play();
	void pause();
	void fit();
	void nextWallpaper();

	bool hasCurrent() const { return !m_wallpaperList.empty(); }
	bool hasPrevious() const { return m_wallpaperList.size() > 1; }

	void loadImage();
	void setLoadedImage();
	void deleteLoadedImage();

	bool canPrevious() const noexcept;
	bool canOpen() const noexcept;
	bool canStop() const noexcept;
	bool canPlay() const noexcept;
	bool canPause() const noexcept;
	bool canFit() const noexcept;
	bool canNext() const noexcept;

private:
	AppState& m_appState;
	const WinUtils& m_winUtils;
	const Settings& m_settings;
	const CollectionManager& m_collectionManager;
	Timer& m_timer;

	std::mutex m_sessionFileAccess;

	const std::filesystem::path pathOfLoaded;
	const std::filesystem::path pathOfCurrent;
	mutable std::mutex m_imageModification;

	std::list<Wallpaper> m_wallpaperList;
	Wallpaper m_nextWallpaper;
	bool m_settingPrevious = false;
};
