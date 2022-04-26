#pragma once

#include <vector>

#include "BaseCollection.h"
#include "CategoriesAndPurity.h"
#include "CollectionManager.h"

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
	UserCollection(CollectionManager* collectionManager);
	~UserCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	Wallpaper* getWallpaperInfo(unsigned int index);
	static bool loadWallpaper(Wallpaper* wallpaper);
	static std::vector<UserCollectionInfo> loadCollectionList(char* username, char* apiKey);
	LPCSTR collectionType() const { return "User collection"; }
	LPCWSTR collectionName() const;
	CollectionType getCollectionType() const { return CollectionType::user; };
	CategoriesAndPurity getCAP();
	void openCollectionSettingsWindow(); 
	static void openWallpaperExternal(Wallpaper* wallpaper);

	UserCollectionSettings* settings;
	
private:
	int per_page = 24;
	char collectionUrl[255] = "";
	CollectionManager* m_pCollectionManager = nullptr;
};