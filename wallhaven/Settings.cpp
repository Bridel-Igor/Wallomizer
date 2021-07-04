#include "Settings.h"
#include <string>
#include <windows.h>

Settings::Settings()
{
	if (!loadSettings())
	{
		MessageBoxA(nullptr, "Can't load settings. Exiting...", "wallhaven", MB_OK);
		exit(0);
	}
}

bool Settings::saveSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings.dat", "wb");
	if (pFile != NULL)
	{
		fwrite(this, sizeof(Settings), 1, pFile);
		fclose(pFile);
		return true;
	}
	MessageBoxA(nullptr, "Can't settings settings.", "wallhaven", MB_OK);
	return false;
}

bool Settings::loadSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings.dat", "rb");
	if (pFile != NULL)
	{
		fread(this, sizeof(Settings), 1, pFile);
		fclose(pFile);
		return true;
	}
	return false;
}