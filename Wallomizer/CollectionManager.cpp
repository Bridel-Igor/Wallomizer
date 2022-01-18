#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <ctime>

#include "CollectionManager.h"
#include "Settings.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "Filesystem.h"
#include "Delay.h"

std::vector<BaseCollection*> CollectionManager::collections;
unsigned int CollectionManager::number=0;
std::list<Wallpaper*> CollectionManager::previous;
bool CollectionManager::bIsReady = false;
bool CollectionManager::bLoading = false;
Wallpaper *CollectionManager::current;
Wallpaper *CollectionManager::next;
std::mt19937 CollectionManager::rndGen(static_cast<unsigned int>(time(0)));
std::uniform_int_distribution<int> CollectionManager::uid(0, 0);

bool CollectionManager::saveSettings()
{
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"CollectionManager.dat");
	FILE* pFile;
	_wfopen_s(&pFile, path, L"w");
	if (pFile != NULL)
	{
		char cNumber[10];
		_itoa_s((int)collections.size(), cNumber, 10);
		cNumber[9] = '\0';
		fputs(cNumber, pFile);
		fputs("\n", pFile);
		for (auto p : collections)
			p->saveSettings(pFile);
		fclose(pFile);
		return true;
	}
	return false;
}

// PERF: don't update number of disabled collections
bool CollectionManager::loadSettings()
{
	bLoading = true;
	if (MainWindow::mainWindow != nullptr)
		MainWindow::mainWindow->player->updateTimer();
	if (TrayWindow::trayWindow != nullptr)
		TrayWindow::trayWindow->player->updateTimer();
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"CollectionManager.dat");
	bIsReady = false;
	FILE* pFile;
	_wfopen_s(&pFile, path, L"r");
	if (pFile != NULL)
	{
		clear();
		size_t n;
		char buffer[255];
		fgets(buffer, 255, pFile);
		buffer[strlen(buffer)-1] = '\0';
		n = atoi(buffer);
		BaseCollection* tmpCollection;
		for (unsigned int i = 0; i < n; i++)
		{
			fgets(buffer, 255, pFile);
			buffer[strlen(buffer) - 1] = '\0';
			if (strcmp(buffer, "Local collection") == 0)
			{
				tmpCollection = new LocalCollection();
				tmpCollection->loadSettings(pFile);
				tmpCollection->isValid = true;
				collections.push_back(tmpCollection);
			}
			if (strcmp(buffer, "User collection") == 0)
			{
				tmpCollection = new UserCollection();
				tmpCollection->loadSettings(pFile);
				tmpCollection->isValid = true;
				collections.push_back(tmpCollection);
			}
			if (strcmp(buffer, "Search collection") == 0)
			{
				tmpCollection = new SearchCollection();
				tmpCollection->loadSettings(pFile);
				tmpCollection->isValid = true;
				collections.push_back(tmpCollection);
			}
		}
		fclose(pFile);
	}
	updateNumber();
	if (MainWindow::mainWindow != nullptr)
		MainWindow::collectionItemsFrame->updateCollectionItems();
	bIsReady = true;
	if (Delay::bRunSlideshow)
		Delay::abortDelay();
	if (number == 0)
	{
		while (TrayWindow::trayWindow == nullptr)
			Sleep(10);
		PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, (WPARAM)TrayWindow::trayWindow->btnSettings->hMenu, NULL);
	}
	bLoading = false;
	if (MainWindow::mainWindow != nullptr)
		MainWindow::mainWindow->player->updateTimer();
	if (TrayWindow::trayWindow != nullptr)
		TrayWindow::trayWindow->player->updateTimer();
	return true;
}

void CollectionManager::reloadSettings()
{
	Delay::beginImageModification();
	saveSettings();
	loadSettings();
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Loaded wallpaper.dat");
	DeleteFileW(path);
	Delay::endImageModification();
	Delay::replayDelay();
}

void CollectionManager::updateNumber()
{
	number = 0;
	for (unsigned int i = 0; i < collections.size(); i++)
		number += collections[i]->isEnabled ? collections[i]->getNumber() : 0;
	if (number>0)
		uid = std::uniform_int_distribution<int>(0, number-1);
}

template <typename T> void CollectionManager::addCollection()
{
	BaseCollection* col = new T;
	collections.push_back(col);
	collections.back()->openCollectionSettingsWindow();
	if (!collections.back()->isValid)
	{
		delete collections.back();
		collections.pop_back();
	}
	else
		reloadSettings();
}
template void CollectionManager::addCollection<LocalCollection>();
template void CollectionManager::addCollection<UserCollection>();
template void CollectionManager::addCollection<SearchCollection>();

void CollectionManager::eraseCollection(int index)
{
	if (collections[index]!=nullptr)
		delete collections[index];
	collections.erase(CollectionManager::collections.begin() + index);
	saveSettings();
	updateNumber();
	Delay::beginImageModification();
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Loaded wallpaper.dat");
	DeleteFileW(path);
	Delay::endImageModification();
	Delay::abortDelay();
}

void CollectionManager::clear()
{
	for (auto p : collections)
		delete p;
	collections.clear();
}

void CollectionManager::loadRandomWallpaper()
{
	if (number <= 0)
		return;
	Delay::beginImageModification();
	int randomFromAll = uid(rndGen);
	getWallpaperInfo(next, randomFromAll);
	loadWallpaper(next);
	Delay::endImageModification();
}

bool CollectionManager::getWallpaperInfo(Wallpaper*& wallpaper, int index)
{
	for (unsigned int i = 0; i < collections.size(); i++)
	{
		if (!collections[i]->isEnabled)
			continue;
		index -= collections[i]->getNumber();
		if (index < 0)
		{
			if (collections[i] == nullptr || i >= collections.size() || collections[i]->getNumber() <= (index + collections[i]->getNumber()))
				return false;
			collections[i]->getWallpaperInfo(wallpaper, index + collections[i]->getNumber());
			return true;
		}
	}
	return false;
}

bool CollectionManager::loadWallpaper(Wallpaper *wallpaper)
{
	switch (wallpaper->getType())
	{
	case CollectionType::local:
		return LocalCollection::loadWallpaper(wallpaper);
	case CollectionType::user:
		return UserCollection::loadWallpaper(wallpaper);
	case CollectionType::search:
		return SearchCollection::loadWallpaper(wallpaper);
	default:
		return false;
	}
}

void CollectionManager::loadNextWallpaper()
{
	loadRandomWallpaper();
}

void CollectionManager::setLoadedWallpaper(bool setPrevious)
{
	wchar_t loadedPath[MAX_PATH], currentPath[MAX_PATH];
	Filesystem::getRoamingDir(loadedPath);
	Filesystem::getRoamingDir(currentPath);
	wcscat_s(loadedPath, MAX_PATH, L"Loaded wallpaper.dat");
	wcscat_s(currentPath, MAX_PATH, L"Current wallpaper.jpg");

	if (!std::experimental::filesystem::exists(loadedPath))
	{
		Delay::abortDelay();
		return;
	}
	Delay::beginImageModification();
	if (!setPrevious)
	{
		if (current != nullptr)
		{
			previous.push_back(current);
			if (previous.size() > Settings::prevCount)
			{
				delete previous.front();
				previous.pop_front();
			}
		}
		current = next;
	}
	DeleteFileW(currentPath);
	if (MoveFileW(loadedPath, currentPath) == 0)
	{
		Delay::endImageModification();
		return;
	}
	wchar_t currentPathNative[MAX_PATH];
	Filesystem::getRoamingDirNative(currentPathNative);
	wcscat_s(currentPathNative, MAX_PATH, L"Current wallpaper.jpg");
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, currentPathNative, SPIF_UPDATEINIFILE);
	Player::redrawPlayers();
	Delay::endImageModification();
}

void CollectionManager::setNextWallpaper()
{
	setLoadedWallpaper();
	loadNextWallpaper();
}

void CollectionManager::setPreviousWallpaper()
{
	if (previous.empty())
		return;

	loadWallpaper(previous.back());
	setLoadedWallpaper(true);
	loadNextWallpaper();

	delete current;
	current = previous.back();
	previous.pop_back();

	Player::redrawPlayers();
}

bool CollectionManager::isPrevious()
{
	return !previous.empty();
}

void CollectionManager::openWallpaperExternal()
{
	switch (current->getType())
	{
	case CollectionType::local:
		LocalCollection::openWallpaperExternal(current);
		break;
	case CollectionType::user:
		UserCollection::openWallpaperExternal(current);
		break;
	case CollectionType::search:
		SearchCollection::openWallpaperExternal(current);
		break;
	default:
		return;
	}
}