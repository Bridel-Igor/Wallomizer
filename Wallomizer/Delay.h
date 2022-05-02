#pragma once

#include "Wallpaper.h"

namespace Delay
{
	extern bool exiting;
	extern bool isSlideshowRunning;

	void saveSession(Wallpaper *pCurrent = nullptr);
	void loadSession(Wallpaper *&pCurrent);
	void delay();
	unsigned long getRemainingDelay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
}