#pragma once

#include <Windows.h>

class Settings
{
	static bool runDelay;

public:
	Settings() = delete;
	Settings(Settings&) = delete;
	void operator=(const Settings&) = delete;
	static void saveSettings();
	static void loadSettings();
	static void abortDelay();
	static void delayAborted();
	static bool canRunDelay();

	static unsigned long delay;
};