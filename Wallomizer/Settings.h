#pragma once

namespace Settings
{
	extern unsigned int prevCount;
	extern unsigned long delay;
	extern char username[64];
	extern char apiKey[128];
	extern bool loadOnStartup;

	void saveSettings();
	void loadSettings();
	void setApiKey(char* apiKey);
	char* getApiKey();
	bool isApiKeyUsed();
}