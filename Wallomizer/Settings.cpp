#include "Settings.h"
#include "Filesystem.h"
#include "MainWindow.h"

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
	constexpr unsigned char settingsVersion = 1;
	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long delayed = 0;
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
		unsigned char fileVersion = 0;
		fwrite(&fileVersion, sizeof(fileVersion), 1, pFile);
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

void Settings::saveSession()
{
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, path, L"wb");
	if (pFile != NULL)
	{
		fwrite(&bRunSlideshow, sizeof(bRunSlideshow), 1, pFile);
		fwrite(&delayed, sizeof(delayed), 1, pFile);
		fclose(pFile);
	}
}

void Settings::loadSession()
{
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, path, L"rb");
	if (pFile != NULL)
	{
		fread(&bRunSlideshow, sizeof(bRunSlideshow), 1, pFile);
		fread(&delayed, sizeof(delayed), 1, pFile);
		fclose(pFile);
	}
	DeleteFileW(path);
}

unsigned long Settings::getRemainingDelay()
{
	return delay > delayed ? delay - delayed : 0;
}

void Settings::Delay()
{
	while (delayed < Settings::delay)
	{
		if (bAbortDelay)
		{
			bAbortDelay = false;
			delayed = 0;
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
		if (MainWindow::mainWindow!=nullptr && delayed%1000 == 0)
			MainWindow::mainWindow->updateTimer();
	}
	delayed = 0;
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