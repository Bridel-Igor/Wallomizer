#pragma once

#include "AppMutex.h"
#include "WinUtils.h"
#include "Timer.h"
#include "CollectionManager.h"
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
	void exit();

	const WinUtils& getWinUtils() const { return m_winUtils; }
	Settings& getSettings() { return m_settings; }
	CollectionManager& getCollectionManager() { return m_collectionManager; }
	Timer& getTimer() { return m_timer; }

private:
	AppMutex m_appMutex;
	WinUtils m_winUtils;
	Settings m_settings;
	CollectionManager m_collectionManager;
	Timer m_timer;

	bool m_running = true;
};
