#include <windows.h>

#include "Delay.h"
#include "Settings.h"
#include "Filesystem.h"
#include "Player.h"

bool Delay::exiting = false;
bool Delay::isSlideshowRunning = true;

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
	fwrite(&isSlideshowRunning, sizeof(isSlideshowRunning), 1, pFile);
	fwrite(&uDelayed, sizeof(uDelayed), 1, pFile);
	CollectionType type = CollectionType::none;
	if (pCurrent)
		type = pCurrent->getType();
	fwrite(&type, sizeof(CollectionType), 1, pFile);
	switch (type)
	{
	case CollectionType::local:		fwrite(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
	case CollectionType::user:		fwrite(pCurrent->getPathA(), sizeof(char), 255, pFile);			break;
	case CollectionType::search:	fwrite(pCurrent->getPathA(), sizeof(char), 1024, pFile);		break;
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
	fread(&isSlideshowRunning, sizeof(isSlideshowRunning), 1, pFile);
	fread(&uDelayed, sizeof(uDelayed), 1, pFile);
	if (pCurrent == nullptr)
	{
		CollectionType type;
		fread(&type, sizeof(CollectionType), 1, pFile);
		pCurrent = new Wallpaper(type);
		switch (type)
		{
		case CollectionType::local:		fread(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
		case CollectionType::user:		fread(pCurrent->getPathA(), sizeof(char), 255, pFile);			break;
		case CollectionType::search:	fread(pCurrent->getPathA(), sizeof(char), 1024, pFile);			break;
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
		if (isSlideshowRunning)
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

void Delay::startSlideshow()
{
	isSlideshowRunning = true;
}

void Delay::pauseSlideshow()
{
	isSlideshowRunning = false;
}