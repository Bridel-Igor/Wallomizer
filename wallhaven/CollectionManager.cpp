#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include "CollectionManager.h"
#include "Settings.h"
#include <string>
#include "SettingsWindow.h"

std::vector<BaseCollection*> CollectionManager::collections;
unsigned int CollectionManager::number=0;

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
	for (auto p : collections)
		delete p;
	collections.clear();
}

bool CollectionManager::setRandomWallpaper()
{
	if (number <= 0)
		return false;
	int randomFromAll = rand() % number;
	for (unsigned int i = 0; i < collections.size(); i++)
	{
		randomFromAll -= collections[i]->getNumber();
		if (randomFromAll < 0)
		{
			if (collections[i] != nullptr && i<=collections.size())
				return collections[i]->setRandomWallpaper();
			else
				return false;
		}
	}
	return false;
}