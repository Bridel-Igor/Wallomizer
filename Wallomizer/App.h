#pragma once


#include "InstanceGuard.h"
#include "AppState.h"
#include "WinUtils.h"
#include "Settings.h"
#include "Timer.h"
#include "CollectionManager.h"
#include "WallpaperManager.h"
#include "UI.h"

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


	AppState& getAppState()						noexcept { return m_appState; }
	const WinUtils& getWinUtils() const			noexcept { return m_winUtils; }
	Settings& getSettings()						noexcept { return m_settings; }
	Timer& getTimer()							noexcept { return m_timer; }
	CollectionManager& getCollectionManager()	noexcept { return m_collectionManager; }
	WallpaperManager& getWallpaperManager()		noexcept { return m_wallpaperManager; }
	UI& getUI()									noexcept { return m_ui; }

private:
	const InstanceGuard m_instanceGuard;
	AppState m_appState;
	const WinUtils m_winUtils;
	Settings m_settings;
	Timer m_timer;
	CollectionManager m_collectionManager;
	WallpaperManager m_wallpaperManager;
	UI m_ui;
};
