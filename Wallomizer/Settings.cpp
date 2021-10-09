#include "Settings.h"
#include "Filesystem.h"

unsigned int Settings::prevCount = 5;
unsigned long Settings::delay = 60000;
std::mutex Settings::loadingImage;
bool Settings::exiting = false;
char Settings::username[64];
char Settings::apiKey[128];
bool Settings::loadOnStartup = false;
bool Settings::bRunSlideshow = true;

namespace Settings
{
	bool bAbortDelay = false;
	bool bReplayDelay = false;
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
	_wfopen_s(&pFile, path, L"r");
	if (pFile != NULL)
	{
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

void Settings::Delay()
{
	unsigned long delayed = 0;
	while (delayed < Settings::delay)
	{
		if (bAbortDelay)
		{
			bAbortDelay = false;
			return;
		}
		if (bReplayDelay)
		{
			bReplayDelay = false;
			delayed = 0;
			continue;
		}
		Sleep(100);
		if (bRunSlideshow)
			delayed += 100;
	}
}

void Settings::abortDelay()
{
	bAbortDelay = true;
}

void Settings::replayDelay()
{
	bReplayDelay = true;
}

void Settings::startSlideshow()
{
	bRunSlideshow = true;
}

void Settings::pauseSlideshow()
{
	bRunSlideshow = false;
}

void Settings::setApiKey(char* _apiKey)
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