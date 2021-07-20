#pragma once

#include "BaseCollection.h"

class DirectoryCollection : public BaseCollection
{
private:
	unsigned int number;
	
public:
	DirectoryCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	virtual bool setWallpaper(unsigned int index);
	LPCSTR collectionType() const { return "Directory collection"; };
	LPCSTR collectionName() const;
	unsigned int getNumber() { return number; }
	void openCollectionSettingsWindow();

	char directoryPath[255];
};