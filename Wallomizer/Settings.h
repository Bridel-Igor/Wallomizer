#pragma once

class WinUtils;

class Settings
{
public:
	Settings(WinUtils& winUtils);
	void saveSettings();
	void loadSettings();
	void setApiKey(const wchar_t* apiKey);
	const wchar_t* getApiKey() const noexcept { return apiKey; }
	const bool isApiKeyUsed() const noexcept { return apiKey[0]; }

	unsigned int prevCount = 5;
	unsigned long delay = 300000;
	wchar_t username[64] = {0};
	wchar_t apiKey[33] = {0};
	bool loadOnStartup = false;
	unsigned int uPerPage = 24;

private:
	WinUtils& m_winUtils;
};
