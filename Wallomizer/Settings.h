#pragma once

class WinUtils;

class Settings
{
public:
	Settings(WinUtils& winUtils);
	void saveSettings();
	void loadSettings();
	void setApiKey(const wchar_t* apiKey);
	wchar_t* getApiKey();
	bool isApiKeyUsed();

	unsigned int prevCount;
	unsigned long delay;
	wchar_t username[64] = {0};
	wchar_t apiKey[33] = {0};
	bool loadOnStartup;
	unsigned int uPerPage;

private:
	WinUtils& m_winUtils;
};