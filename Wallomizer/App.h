#pragma once

#include "AppMutex.h"
#include "WinUtils.h"
#include "Delay.h"
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

	const WinUtils& getWinUtils() const { return m_winUtils; }
	Delay& getDelay() { return m_delay; }
	CollectionManager& getCollectionManager() { return m_collectionManager; }
	Settings& getSettings() { return m_settings; }

private:
	AppMutex m_appMutex;
	WinUtils m_winUtils;
	Delay m_delay;
	CollectionManager m_collectionManager;
	Settings m_settings;
};
