#include "Settings.h"

#include <Windows.h>
#include <stdio.h>

#include "Internet.h"
#include "WinUtils.h"

constexpr unsigned short SETTINGS_FILE_VERSION = 3U;

Settings::Settings(WinUtils& winUtils) :
	m_winUtils(winUtils)
{
	prevCount = 5;
	delay = 300000;
	loadOnStartup = false;
	uPerPage = 24;
}

void Settings::saveSettings()
{
	wchar_t wcPath[MAX_PATH];
	wcscpy_s(wcPath, MAX_PATH, m_winUtils.getRoamingDir());
	wcscat_s(wcPath, MAX_PATH, L"Settings.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wcPath, L"wb");
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
	wchar_t wcPath[MAX_PATH];
	wcscpy_s(wcPath, MAX_PATH, m_winUtils.getRoamingDir());
	wcscat_s(wcPath, MAX_PATH, L"Settings.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wcPath, L"rb");
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
		
		if (isApiKeyUsed())
		{
			// get user's per page
			Internet internet; // TODO: think about no internet exceptions!
			wchar_t wsURL[128] = L"https://wallhaven.cc/api/v1/settings?apikey=";
			wcscat_s(wsURL, apiKey);
			internet.DownloadToBuffer(wsURL, 64);
			wchar_t wsPerPage[4];
			internet.parse("per_page", wsPerPage);
			uPerPage = wcstoul(wsPerPage, nullptr, 10);
		}

		return;
	}
	saveSettings();
}

void Settings::setApiKey(const wchar_t* _apiKey)
{
	wcscpy_s(apiKey, 33, _apiKey);
}

wchar_t* Settings::getApiKey()
{
	return apiKey;
}

bool Settings::isApiKeyUsed()
{
	return apiKey[0];
}