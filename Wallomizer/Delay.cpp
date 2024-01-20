#include <windows.h>

#include "Delay.h"
#include "Settings.h"
#include "Filesystem.h"
#include "Player.h"

bool Delay::exiting = false;
Delay::SlideshowStatus Delay::slideshowStatus = Delay::SlideshowStatus::playing;

namespace Delay
{
	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long uDelayed = 0;
}

void Delay::saveSession(Wallpaper *pCurrent)
{
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wsPath, L"wb");
	if (pFile == NULL)
		return;
	fwrite(&slideshowStatus, sizeof(slideshowStatus), 1, pFile);
	fwrite(&uDelayed, sizeof(uDelayed), 1, pFile);
	CollectionType type = CollectionType::none;
	if (pCurrent)
		type = pCurrent->getType();
	fwrite(&type, sizeof(CollectionType), 1, pFile);
	switch (type)
	{
	case CollectionType::local:		fwrite(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
	case CollectionType::user:		fwrite(pCurrent->getPathW(), sizeof(wchar_t), 255, pFile);		break;
	case CollectionType::search:	fwrite(pCurrent->getPathW(), sizeof(wchar_t), 1024, pFile);		break;
	}
	fclose(pFile);
}

void Delay::loadSession(Wallpaper*& pCurrent)
{
	wchar_t wsPath[MAX_PATH];
	Filesystem::getRoamingDir(wsPath);
	wcscat_s(wsPath, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wsPath, L"rb");
	if (pFile == NULL)
		return;
	fread(&slideshowStatus, sizeof(slideshowStatus), 1, pFile);
	fread(&uDelayed, sizeof(uDelayed), 1, pFile);
	if (pCurrent == nullptr)
	{
		CollectionType type;
		fread(&type, sizeof(CollectionType), 1, pFile);
		pCurrent = new Wallpaper(type);
		switch (type)
		{
		case CollectionType::local:		fread(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
		case CollectionType::user:		fread(pCurrent->getPathW(), sizeof(wchar_t), 255, pFile);		break;
		case CollectionType::search:	fread(pCurrent->getPathW(), sizeof(wchar_t), 1024, pFile);		break;
		}
	}
	fclose(pFile);
	bAbortDelay = false;
	DeleteFileW(wsPath);
}

void Delay::delay()
{
	while (uDelayed < Settings::delay)
	{
		if (bAbortDelay)
		{
			bAbortDelay = false;
			uDelayed = 0;
			return;
		}
		if (bReplayDelay)
		{
			bReplayDelay = false;
			uDelayed = 0;
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (slideshowStatus == SlideshowStatus::playing)
			uDelayed += 100;
		if (uDelayed % 1000 == 0)
			Player::updateTimer();
	}
	uDelayed = 0;
}

unsigned long Delay::getRemainingDelay()
{
	return Settings::delay > uDelayed ? Settings::delay - uDelayed : 0;
}

void Delay::abortDelay()
{
	bAbortDelay = true;
}

void Delay::replayDelay()
{
	bReplayDelay = true;
}

void Delay::setSlideshowStatus(const SlideshowStatus status)
{
	slideshowStatus = status;

	if (status == SlideshowStatus::stopped)
	{
		SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID)&L"", SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
	}
	else
	{
		wchar_t wsCurrentPathNative[MAX_PATH];
		Filesystem::getRoamingDirNative(wsCurrentPathNative);
		wcscat_s(wsCurrentPathNative, MAX_PATH, L"Current wallpaper.jpg");
		SystemParametersInfoW(SPI_SETDESKWALLPAPER, 3, wsCurrentPathNative, SPIF_UPDATEINIFILE);
		Player::redrawPlayers();
	}
}