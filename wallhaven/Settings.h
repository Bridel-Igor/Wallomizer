#pragma once

#include <Windows.h>
#include <mutex>

namespace Settings
{
	extern unsigned int prevCount;
	extern unsigned long delay;
	extern std::mutex slideshow;

	void saveSettings();
	void loadSettings();
	void Delay();
	void abortDelay();
	void replayDelay();
}