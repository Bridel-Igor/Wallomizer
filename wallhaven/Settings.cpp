#include "Settings.h"
#include <string>

// external variables
unsigned int Settings::prevCount = 5;
unsigned long Settings::delay = 60000;
std::mutex Settings::slideshow;

// internal variables
namespace Settings
{
	bool bAbortDelay = true;
	bool bReplayDelay = false;
}

void Settings::saveSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings.dat", "wb");
	if (pFile != NULL)
	{
		fwrite(&delay, sizeof(delay), 1, pFile);
		fclose(pFile);
		return;
	}
	MessageBoxA(nullptr, "Can't save settings.", "wallhaven", MB_OK);
}

void Settings::loadSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings.dat", "rb");
	if (pFile != NULL)
	{
		fread(&delay, sizeof(delay), 1, pFile);
		fclose(pFile);
		return;
	}
	MessageBoxA(nullptr, "Can't load settings.", "wallhaven", MB_OK);
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
		delayed += 100;
		slideshow.lock();
		slideshow.unlock();
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