#pragma once

#include <Windows.h>
#include <mutex>

namespace Settings
{
	extern unsigned int prevCount;
	extern unsigned long delay;
	extern bool exiting;
	extern std::mutex loadingImage;

	void saveSettings();
	void loadSettings();
	void Delay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
}