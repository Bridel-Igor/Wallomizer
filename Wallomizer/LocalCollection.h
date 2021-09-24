#pragma once

#include "BaseCollection.h"

class LocalCollection : public BaseCollection
{
public:
	LocalCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	virtual bool loadWallpaper(unsigned int index);
	LPCSTR collectionType() const { return "Local collection"; };
	LPCSTR collectionName() const;
	void openCollectionSettingsWindow();
	void openWallpaperExternal(unsigned int index);

	char directoryPath[255];
};