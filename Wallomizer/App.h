#pragma once

#include "AppMutex.h"
#include "WinUtils.h"
#include "Timer.h"
#include "CollectionManager.h"
#include "WallpaperManager.h"
#include "Settings.h"

/// Root application class responsible for managing the application lifecycle
/// and owning all major subsystems.
/// Only one instance of App is expected to exist during the application lifetime.
class App
{
public:
	App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	App(App&&) = delete;
	App& operator=(App&&) = delete;

	/// Runs the complete application lifecycle.
	int run();
	void requestExit();

	const WinUtils& getWinUtils() const			noexcept { return m_winUtils; }
	Settings& getSettings()						noexcept { return m_settings; }
	CollectionManager& getCollectionManager()	noexcept { return m_collectionManager; }
	WallpaperManager& getWallpaperManager()		noexcept { return m_wallpaperManager; }
	Timer& getTimer()							noexcept { return m_timer; }

private:
	const AppMutex m_appMutex;
	const WinUtils m_winUtils;
	Settings m_settings;
	CollectionManager m_collectionManager;
	WallpaperManager m_wallpaperManager;
	Timer m_timer;

	bool m_running = true;
};
