#pragma once

namespace Delay
{
	extern bool exiting;
	extern bool bRunSlideshow;

	void saveSession();
	void loadSession();
	unsigned long getRemainingDelay();
	void Delay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
	void beginImageModification();
	void endImageModification();
}