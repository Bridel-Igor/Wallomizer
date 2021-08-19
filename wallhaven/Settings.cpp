#include "Settings.h"

unsigned int Settings::prevCount = 5;
unsigned long Settings::delay = 60000;
std::mutex Settings::loadingImage;
bool Settings::exiting = false;
char Settings::username[64];
char Settings::apiKey[128];

namespace Settings
{
	bool bAbortDelay = false;
	bool bReplayDelay = false;
	bool bRunSlideshow = true;
}

void Settings::saveSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings/Settings.dat", "w");
	if (pFile != NULL)
	{
		char cNumber[10];
		_itoa_s(delay, cNumber, 10);
		cNumber[9] = '\0';
		fputs(cNumber, pFile);
		fputs("\n", pFile);
		fputs(username, pFile);
		fputs("\n", pFile);
		fputs(apiKey, pFile);
		fputs("\n", pFile);
		fclose(pFile);
		return;
	}
	MessageBoxA(nullptr, "Can't save settings.", "Wallhaven", MB_OK | MB_ICONEXCLAMATION);
}

void Settings::loadSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings/Settings.dat", "r");
	if (pFile != NULL)
	{
		char buffer[10];
		fgets(buffer, 10, pFile);
		buffer[strlen(buffer) - 1] = '\0';
		delay = atoi(buffer);
		fgets(username, 64, pFile);
		username[strlen(username) - 1] = '\0';
		fgets(apiKey, 128, pFile);
		apiKey[strlen(apiKey) -1] = '\0';
		fclose(pFile);
		return;
	}
	MessageBoxA(nullptr, "Can't load settings.", "Wallhaven", MB_OK | MB_ICONEXCLAMATION);
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