#pragma once

#include <Windows.h>
#include <mutex>

namespace Settings
{
	extern unsigned int prevCount;
	extern unsigned long delay;
	extern bool exiting;
	extern std::mutex loadingImage;
	extern char username[64];
	extern char apiKey[128];

	void saveSettings();
	void loadSettings();
	void Delay();
	void abortDelay();
	void replayDelay();
	void startSlideshow();
	void pauseSlideshow();
	void setApiKey(char* apiKey);
	char* getApiKey();
	bool isApiKeyUsed();
}