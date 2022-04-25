#pragma once

#include <Windows.h>
#include <string>

#include "Wallpaper.h"
#include "CategoriesAndPurity.h"

class BaseCollection
{
public:
	virtual bool saveSettings(FILE *pFile) = 0;
	virtual bool loadSettings(FILE *pFile) = 0;
	virtual Wallpaper* getWallpaperInfo(unsigned int index) = 0;
	virtual LPCSTR collectionType() const = 0;
	virtual LPCSTR collectionName() const = 0;
	virtual CategoriesAndPurity getCAP() = 0;
	virtual unsigned int getNumber() { return number; }
	virtual void openCollectionSettingsWindow() = 0;
	
	bool isEnabled = true;
	bool isValid = false;

protected:
	unsigned int number = 0;
};