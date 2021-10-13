#include <windows.h>

#include "Delay.h"
#include "Settings.h"
#include "Filesystem.h"
#include "MainWindow.h"

std::mutex Delay::loadingImage;
bool Delay::exiting = false;
bool Delay::bRunSlideshow = true;

namespace Delay
{
	bool bAbortDelay = false;
	bool bReplayDelay = false;
	unsigned long delayed = 0;
}

void Delay::saveSession()
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

void Delay::loadSession()
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
		if (MainWindow::mainWindow != nullptr && delayed % 1000 == 0)
			MainWindow::mainWindow->updateTimer();
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