#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <string>
#include <time.h>

#include "CollectionManager.h"
#include "Settings.h"
#include "MainWindow.h"
#include "TrayWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"

std::vector<BaseCollection*> CollectionManager::collections;
unsigned int CollectionManager::number=0;
int *CollectionManager::previous = nullptr;
int CollectionManager::indexOfLoaded = 0;
bool CollectionManager::bIsReady = false;

bool CollectionManager::saveSettings()
{
	FILE* pFile;
	fopen_s(&pFile, "Settings/CollectionManager.dat", "w");
	if (pFile != NULL)
	{
		char cNumber[10];
		_itoa_s(collections.size(), cNumber, 10);
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

bool CollectionManager::loadSettings()
{
	bIsReady = false;
	FILE* pFile;
	fopen_s(&pFile, "Settings/CollectionManager.dat", "r");
	if (pFile != NULL)
	{
		clear();
		previous = new int[Settings::prevCount+1];
		for (unsigned int i = 0; i < Settings::prevCount+1; i++)
			previous[i] = -1;
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
	Settings::abortDelay();
	srand((unsigned int)time(NULL));
	if (number == 0)
	{
		Sleep(10);
		PostMessageA(TrayWindow::trayWindow->Window(), WM_COMMAND, (WPARAM)ID_WALLOMIZER_SETTINGS, NULL);
	}
	return true;
}

void CollectionManager::reloadSettings()
{
	Settings::loadingImage.lock();
	saveSettings();
	loadSettings();
	remove("Resources/Loaded wallpaper.dat");
	Settings::loadingImage.unlock();
}

void CollectionManager::updateNumber()
{
	number = 0;
	for (unsigned int i = 0; i < collections.size(); i++)
		number += collections[i]->isEnabled ? collections[i]->getNumber() : 0;
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
	Settings::loadingImage.lock();
	remove("Resources/Loaded wallpaper.dat");
	Settings::loadingImage.unlock();
	Settings::abortDelay();
}

void CollectionManager::clear()
{
	if (previous!=nullptr)
	{
		delete [] previous;
		previous = nullptr;
	}
	for (auto p : collections)
		delete p;
	collections.clear();
}

void CollectionManager::loadRandomWallpaper()
{
	if (number <= 0)
		return;
	Settings::loadingImage.lock();
	int randomFromAll = rand() % number;
	if (!loadWallpaper(randomFromAll))
		loadWallpaper(randomFromAll);
	Settings::loadingImage.unlock();
}

bool CollectionManager::loadWallpaper(int index)
{
	int tmpIndex = index;
	indexOfLoaded = index;
	for (unsigned int i = 0; i < collections.size(); i++)
	{
		if (!collections[i]->isEnabled)
			continue;
		index -= collections[i]->getNumber();
		if (index < 0)
		{
			if (collections[i] == nullptr || i >= collections.size() || collections[i]->getNumber() <= (index + collections[i]->getNumber()))
				return false;
			return collections[i]->loadWallpaper(index + collections[i]->getNumber());
		}
	}
	return false;
}

void CollectionManager::loadNextWallpaper()
{
	loadRandomWallpaper();
}

void CollectionManager::setLoadedWallpaper(bool setPrevious)
{
	if (!std::experimental::filesystem::exists("Resources/Loaded wallpaper.dat"))
	{
		Settings::abortDelay();
		return;
	}
	Settings::loadingImage.lock();
	if (!setPrevious && previous != nullptr)
	{
		for (int i = Settings::prevCount; i > 0; i--)
			previous[i] = previous[i - 1];
		previous[0] = indexOfLoaded;
	}
	remove("Resources/Current wallpaper.jpg");
	if (rename("Resources/Loaded wallpaper.dat", "Resources/Current wallpaper.jpg"))
	{
		Settings::loadingImage.unlock();
		return;
	}
	char imgPath[255];
	GetCurrentDirectoryA(255, imgPath);
	strcat_s(imgPath, "\\Resources\\Current wallpaper.jpg");
	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, imgPath, SPIF_UPDATEINIFILE);
	if (MainWindow::mainWindow && !setPrevious)
		InvalidateRect(MainWindow::mainWindow->Window(), NULL, FALSE);
	Settings::loadingImage.unlock();
}

void CollectionManager::setNextWallpaper()
{
	setLoadedWallpaper();
	
	loadNextWallpaper();
}

void CollectionManager::setPreviousWallpaper()
{
	if (previous == nullptr || previous[1] == -1)
		return;
	loadWallpaper(previous[1]);
	setLoadedWallpaper(true);
	loadNextWallpaper();
	for (unsigned int i = 0; i < Settings::prevCount; i++)
		previous[i] = previous[i + 1];
	previous[Settings::prevCount] = -1;
	if (MainWindow::mainWindow)
		InvalidateRect(MainWindow::mainWindow->Window(), NULL, FALSE);
}

bool CollectionManager::isPrevious()
{
	if (previous == nullptr)
		return false;
	return previous[1] != -1 ? true : false;
}

void CollectionManager::openWallpaperExternal()
{
	int index = previous[0];
	for (unsigned int i = 0; i < collections.size(); i++)
	{
		if (!collections[i]->isEnabled)
			continue;
		index -= collections[i]->getNumber();
		if (index < 0)
		{
			if (collections[i] == nullptr || i >= collections.size() || collections[i]->getNumber() <= (index + collections[i]->getNumber()))
				return;
			collections[i]->openWallpaperExternal(index + collections[i]->getNumber());
		}
	}
	return;
}