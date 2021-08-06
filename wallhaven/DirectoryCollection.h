#pragma once

#include "BaseCollection.h"

class DirectoryCollection : public BaseCollection
{
public:
	DirectoryCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	virtual bool loadWallpaper(unsigned int index);
	LPCSTR collectionType() const { return "Directory collection"; };
	LPCSTR collectionName() const;
	void openCollectionSettingsWindow();

	char directoryPath[255];
};