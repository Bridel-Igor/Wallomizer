#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "CollectionManager.h"
#include "Settings.h"
#include <string>
#include "time.h"
#include "SettingsWindow.h"

std::vector<BaseCollection*> CollectionManager::collections;
unsigned int CollectionManager::number=0;
int *CollectionManager::previous = nullptr;

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
	number = 0;
	for (unsigned int i = 0; i < collections.size(); i++)
		number += collections[i]->getNumber();
	if (SettingsWindow::settingsWindow != nullptr)
		SettingsWindow::settingsWindow->updateCollectionItems();
	Settings::abortDelay();
	return true;
}

void CollectionManager::reloadSettings()
{
	saveSettings();
	loadSettings();
}

void CollectionManager::eraseCollection(int index)
{
	if (collections[index]!=nullptr)
		delete collections[index];
	collections.erase(CollectionManager::collections.begin() + index);
	reloadSettings();
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
	for (const auto& entry : std::experimental::filesystem::directory_iterator("Resources"))
		std::experimental::filesystem::remove_all(entry.path());
}

bool CollectionManager::setRandomWallpaper()
{
	srand((unsigned int)time(NULL));
	if (number <= 0)
		return false;
	int randomFromAll = rand() % number;
	if (!setWallpaper(randomFromAll))
		return setWallpaper(randomFromAll);
	return true;
}

bool CollectionManager::setWallpaper(int index, bool setPrevious)
{
	int tmpIndex = index;
	for (unsigned int i = 0; i < collections.size(); i++)
	{
		index -= collections[i]->getNumber();
		if (index < 0)
		{
			if (collections[i] == nullptr || i >= collections.size() || collections[i]->getNumber() <= (index + collections[i]->getNumber()))
				return false;
			if (!setPrevious)
			{
				if (previous != nullptr)
				{
					for (int i = Settings::prevCount; i > 0; i--)
						previous[i] = previous[i - 1];
					previous[0] = tmpIndex;
				}
			}
			return collections[i]->setWallpaper(index + collections[i]->getNumber());
		}
	}
	return false;
}

void CollectionManager::setNextWallpaper()
{
	Settings::replayDelay();
	setRandomWallpaper();
}

void CollectionManager::setPreviousWallpaper()
{
	Settings::replayDelay();
	if (previous == nullptr)
		return;
	if (previous[1]!=-1)
		setWallpaper(previous[1], true);
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