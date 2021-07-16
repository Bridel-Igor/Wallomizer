#include "Settings.h"
#include <string>

unsigned long Settings::delay = 60000;
bool Settings::runDelay = true;

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

void Settings::abortDelay()
{
	runDelay = false;
}

void Settings::delayAborted()
{
	runDelay = true;
}

bool Settings::canRunDelay()
{
	return runDelay;
}