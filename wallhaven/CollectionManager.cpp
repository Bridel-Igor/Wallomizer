#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <string>
#include <time.h>

#include "CollectionManager.h"
#include "Settings.h"
#include "SettingsWindow.h"
#include "UserCollection.h"
#include "DirectoryCollection.h"

std::vector<BaseCollection*> CollectionManager::collections;
unsigned int CollectionManager::number=0;
int *CollectionManager::previous = nullptr;
int CollectionManager::indexOfLoaded = 0;

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
			if (strcmp(buffer, "Directory collection") == 0)
			{
				tmpCollection = new DirectoryCollection();
				tmpCollection->loadSettings(pFile);
				collections.push_back(tmpCollection);
			}
			if (strcmp(buffer, "User collection") == 0)
			{
				tmpCollection = new UserCollection();
				tmpCollection->loadSettings(pFile);
				collections.push_back(tmpCollection);
			}
		}
		fclose(pFile);
	}
	updateNumber();
	if (SettingsWindow::settingsWindow != nullptr)
		SettingsWindow::collectionItemsFrame->updateCollectionItems();
	Settings::abortDelay();
	srand((unsigned int)time(NULL));
	return true;
}

void CollectionManager::reloadSettings()
{
	Settings::loadingImage.lock();
	saveSettings();
	loadSettings();
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
	col->openCollectionSettingsWindow();
	if (strcmp(collections.back()->collectionName(), "") == 0) // erase new collection if it's empty
		collections.pop_back();
}
template void CollectionManager::addCollection<DirectoryCollection>();
template void CollectionManager::addCollection<UserCollection>();

void CollectionManager::eraseCollection(int index)
{
	if (collections[index]!=nullptr)
		delete collections[index];
	collections.erase(CollectionManager::collections.begin() + index);
	saveSettings();
	updateNumber();
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
}

bool CollectionManager::isPrevious()
{
	if (previous == nullptr)
		return false;
	return previous[1] != -1 ? true : false;
}