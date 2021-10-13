#pragma once

#include <mutex>

namespace Delay
{
	extern bool exiting;
	extern std::mutex loadingImage;
	extern bool bRunSlideshow;

	void saveSession();
	void loadSession();
	unsigned long getRemainingDelay();
	void Delay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
}