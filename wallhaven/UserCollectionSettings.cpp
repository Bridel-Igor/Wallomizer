#include "UserCollectionSettings.h"
#include <string>
#include <windows.h>
#include "SetUserCollectionWindow.h"

unsigned long UserCollectionSettings::delay = 60000;

bool UserCollectionSettings::saveSettings()
{
	bool success = true;
	FILE* pFile;
	fopen_s(&pFile, "UserCollectionSettings.dat", "wb");
	if (pFile != NULL)
	{
		fwrite(this, sizeof(UserCollectionSettings), 1, pFile);
		fclose(pFile);
	}
	else
		success = false;
	fopen_s(&pFile, "Settings.dat", "wb");
	if (pFile != NULL)
	{
		fwrite(&delay, sizeof(delay), 1, pFile);
		fclose(pFile);
	}
	else
		success = false;
	if (!success)
		MessageBoxA(nullptr, "Can't save settings.", "wallhaven", MB_OK);
	return success;
}

void UserCollectionSettings::loadSettings()
{
	bool success = true;
	FILE* pFile;
	fopen_s(&pFile, "UserCollectionSettings.dat", "rb");
	if (pFile != NULL)
	{
		fread(this, sizeof(UserCollectionSettings), 1, pFile);
		fclose(pFile);
	}
	else
		success = false;
	fopen_s(&pFile, "Settings.dat", "rb");
	if (pFile != NULL)
	{
		fread(&delay, sizeof(delay), 1, pFile);
		fclose(pFile);
	}
	else
		success = false;
	if (!success)
	{
		SetUserCollectionWindow::windowThread();
		loadSettings();
	}
}