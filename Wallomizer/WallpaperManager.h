#pragma once

#include <atomic>
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

	// Lifecycle
	void loadImage();
	void applyLoadedImage();
	void onCollectionsChanged();
	
	// Controls
	void previousWallpaper();
	void openCurrentWallpaperExternally();
	void stop();
	void play();
	void pause();
	void fit();
	void nextWallpaper();
	
	// Checks
	bool canPrevious() const noexcept;
	bool canOpen() const noexcept;
	bool canStop() const noexcept;
	bool canPlay() const noexcept;
	bool canPause() const noexcept;
	bool canFit() const noexcept;
	bool canNext() const noexcept;
	
	// Session
	bool saveSession();

private:
	bool loadSession();
	void clear();
	
	// Files
	bool removeLoadedFile() noexcept;
	bool removeCurrentFile() noexcept;
	bool makeLoadedFileCurrent() noexcept;
	bool hasLoadedFile() const noexcept;
	
	bool hasCurrent() const noexcept { return !m_wallpaperList.empty() && m_wallpaperList.back().isValid(); }
	bool hasPrevious() const noexcept { return m_wallpaperList.size() > 1; }

	AppState& m_appState;
	const WinUtils& m_winUtils;
	const Settings& m_settings;
	const CollectionManager& m_collectionManager;
	Timer& m_timer;

	const std::filesystem::path m_pathOfLoaded;
	const std::filesystem::path m_pathOfCurrent;

	std::list<Wallpaper> m_wallpaperList;
	Wallpaper m_nextWallpaper;
	std::atomic_bool m_isLoadedImageReady = false;

	std::mutex m_imageModification;
	std::mutex m_sessionFileAccess;
};
