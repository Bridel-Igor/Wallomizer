#include <windows.h>
#include <mutex>

#include "Delay.h"
#include "Settings.h"
#include "Filesystem.h"
#include "Player.h"

bool Delay::exiting = false;
bool Delay::bRunSlideshow = true;

namespace Delay
{
	std::mutex loadingImage;
	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long delayed = 0;
}

void Delay::saveSession(Wallpaper *current)
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
		unsigned char type;
		if (current != nullptr)
		{
			type = static_cast<unsigned char>(current->getType());
			fwrite(&type, sizeof(CollectionType), 1, pFile);
			if (current->getType() == CollectionType::local)
				fwrite(current->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);
			if (current->getType() == CollectionType::user)
				fwrite(current->getPathA(), sizeof(char), 255, pFile);
			if (current->getType() == CollectionType::search)
				fwrite(current->getPathA(), sizeof(char), 1024, pFile);
		}
		else
		{
			type = static_cast<unsigned char>(CollectionType::none);
			fwrite(&type, sizeof(CollectionType), 1, pFile);
		}
		fclose(pFile);
	}
}

void Delay::loadSession(Wallpaper*& current)
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
		if (current == nullptr)
		{
			unsigned char type;
			fread(&type, sizeof(CollectionType), 1, pFile);
			current = new Wallpaper(static_cast<CollectionType>(type));
			if (current->getType() == CollectionType::local)
				fread(current->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);
			if (current->getType() == CollectionType::user)
				fread(current->getPathA(), sizeof(char), 255, pFile);
			if (current->getType() == CollectionType::search)
				fread(current->getPathA(), sizeof(char), 1024, pFile);
		}
		fclose(pFile);
		bAbortDelay = false;
	}
	DeleteFileW(path);
}

unsigned long Delay::getRemainingDelay()
{
	return Settings::delay > delayed ? Settings::delay - delayed : 0;
}

void Delay::Delay()
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
		if (delayed % 1000 == 0)
			Player::updateTimer();
	}
	delayed = 0;
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
	bRunSlideshow = true;
}

void Delay::pauseSlideshow()
{
	bRunSlideshow = false;
}

void Delay::beginImageModification()
{
	Delay::loadingImage.lock();
}

void Delay::endImageModification()
{
	Delay::loadingImage.unlock();
}