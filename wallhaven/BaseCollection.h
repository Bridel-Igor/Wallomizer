#pragma once

#include <Windows.h>
#include <string>

class BaseCollection
{
public:
	virtual bool saveSettings(FILE *pFile) = 0;
	virtual bool loadSettings(FILE *pFile) = 0;
	virtual bool loadWallpaper(unsigned int index) = 0;
	virtual LPCSTR collectionType() const = 0;
	virtual LPCSTR collectionName() const = 0;
	virtual unsigned int getNumber() = 0;
	virtual void openCollectionSettingsWindow() = 0;

	bool isEnabled;
};