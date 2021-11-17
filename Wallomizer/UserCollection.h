#pragma once

#include <vector>

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class UserCollection : public BaseCollection
{
private:
	struct UserCollectionSettings
	{
		char username[64];
		char collectionID[16];
		char collectionName[64];
		CategoriesAndPurity categoriesAndPurity;
	};

public:
	struct UserCollectionInfo
	{
		unsigned int id;
		char label[64];
	};

public:
	UserCollection();
	~UserCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool getWallpaperInfo(Wallpaper*& wallpaper, unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	static std::vector<UserCollectionInfo> loadCollectionList(char* username, char* apiKey);
	LPCSTR collectionType() const { return "User collection"; }
	LPCSTR collectionName() const;
	void openCollectionSettingsWindow(); 
	static void openWallpaperExternal(Wallpaper* wallpaper);

	UserCollectionSettings* settings;
	
private:
	int per_page = 24;
	char collectionUrl[255] = "";
};