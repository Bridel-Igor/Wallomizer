#pragma once

namespace Settings
{
	extern unsigned int prevCount;
	extern unsigned long delay;
	extern wchar_t username[64];
	extern wchar_t apiKey[33];
	extern bool loadOnStartup;
	extern unsigned int uPerPage;

	void saveSettings();
	void loadSettings();
	void setApiKey(const wchar_t* apiKey);
	wchar_t* getApiKey();
	bool isApiKeyUsed();
}