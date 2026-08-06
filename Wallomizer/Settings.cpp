#include "Settings.h"

#include <Windows.h>
#include <stdio.h>

#include "Internet.h"
#include "WinUtils.h"

constexpr unsigned short SETTINGS_FILE_VERSION = 3U;

Settings::Settings(const WinUtils& winUtils) :
	m_winUtils(winUtils)
{
	loadSettings();
}

void Settings::saveSettings() const
{
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Settings.dat";
	FILE* pFile;
	_wfopen_s(&pFile, filePath.c_str(), L"wb");
	if (pFile != NULL)
	{
		fwrite(&SETTINGS_FILE_VERSION, sizeof(SETTINGS_FILE_VERSION), 1, pFile);
		fwrite(&loadOnStartup, sizeof(loadOnStartup), 1, pFile);
		fwrite(&delay, sizeof(delay), 1, pFile);
		fwrite(&username, sizeof(username), 1, pFile);
		fwrite(&apiKey, sizeof(apiKey), 1, pFile);
		fclose(pFile);
		return;
	}
	MessageBoxA(nullptr, "Can't save settings.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
}

void Settings::loadSettings()
{
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Settings.dat";
	FILE* pFile;
	_wfopen_s(&pFile, filePath.c_str(), L"rb");
	if (pFile != NULL)
	{
		unsigned short fileVersion = 0;
		fread(&fileVersion, sizeof(fileVersion), 1, pFile);
		if (fileVersion != SETTINGS_FILE_VERSION)
		{
			fclose(pFile);
			saveSettings();
			MessageBox(NULL, "Incompatible settings file. Settings were reset.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		fread(&loadOnStartup, sizeof(loadOnStartup), 1, pFile);
		fread(&delay, sizeof(delay), 1, pFile);
		fread(&username, sizeof(username), 1, pFile);
		fread(&apiKey, sizeof(apiKey), 1, pFile);
		if (delay < 10000)
			delay = 10000;
		fclose(pFile);	
		
		return;
	}
	saveSettings();
}

void Settings::setApiKey(const wchar_t* _apiKey)
{
	wcscpy_s(apiKey, 33, _apiKey);
}
