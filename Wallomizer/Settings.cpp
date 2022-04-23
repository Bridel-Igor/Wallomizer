#include <Windows.h>
#include <stdio.h>

#include "Settings.h"
#include "Filesystem.h"

unsigned int Settings::prevCount = 5;
unsigned long Settings::delay = 300000;
char Settings::username[64];
char Settings::apiKey[128];
bool Settings::loadOnStartup = false;

namespace Settings
{
	constexpr unsigned short settingsVersion = 1U;
}

void Settings::saveSettings()
{
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Settings.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, path, L"wb");
	if (pFile != NULL)
	{
		fwrite(&settingsVersion, sizeof(settingsVersion), 1, pFile);
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
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Settings.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, path, L"rb");
	if (pFile != NULL)
	{
		unsigned short fileVersion = 0;
		fread(&fileVersion, sizeof(fileVersion), 1, pFile);
		if (fileVersion != settingsVersion)
		{
			fclose(pFile);
			saveSettings();
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

void Settings::setApiKey(const char* _apiKey)
{
	strcpy_s(apiKey, 64, _apiKey);
}

char* Settings::getApiKey()
{
	return apiKey;
}

bool Settings::isApiKeyUsed()
{
	return strlen(apiKey);
}