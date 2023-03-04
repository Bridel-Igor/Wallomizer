#pragma once

#include "Wallpaper.h"

namespace Delay
{
	enum class SlideshowStatus : unsigned char
	{
		paused,
		playing,
		stopped
	};

	extern bool exiting;
	extern SlideshowStatus slideshowStatus;

	void saveSession(Wallpaper *pCurrent = nullptr);
	void loadSession(Wallpaper *&pCurrent);
	void delay();
	unsigned long getRemainingDelay();
	void abortDelay();
	void replayDelay();
	void setSlideshowStatus(const SlideshowStatus status);
}