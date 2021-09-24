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
	virtual unsigned int getNumber() { return number; }
	virtual void openCollectionSettingsWindow() = 0;
	virtual void openWallpaperExternal(unsigned int index) = 0;
	
	bool isEnabled = true;
	bool isValid = false;

protected:
	unsigned int number = 0;
};