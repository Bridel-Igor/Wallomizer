#pragma once

#include "Wallpaper.h"

namespace Delay
{
	extern bool exiting;
	extern bool bRunSlideshow;

	void saveSession(Wallpaper *current = nullptr);
	void loadSession(Wallpaper *&current);
	unsigned long getRemainingDelay();
	void Delay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
	void beginImageModification();
	void endImageModification();
}