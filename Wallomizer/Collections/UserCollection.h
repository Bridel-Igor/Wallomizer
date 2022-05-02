#pragma once

#include <vector>

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"
#include "CollectionManager.h"

class UserCollection : public BaseCollection
{
public:
	struct UserCollectionInfo
	{
		unsigned int id;
		char sLabel[64];
	};

private:
	struct UserCollectionSettings
	{
		char sUsername[64] = "";
		char sCollectionID[16] = "";
		char sCollectionName[64] = "";
		CategoriesAndPurity categoriesAndPurity = CAP::puritySFW;
	};

public:
	UserCollection(CollectionManager* collectionManager) :
		m_pCollectionManager(collectionManager)
	{}

	~UserCollection() {};
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	CollectionType getCollectionType() const { return CollectionType::user; }
	CategoriesAndPurity getCAP() const;
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(const Wallpaper* pWallpaper);
	static void openWallpaperExternal(const Wallpaper* pWallpaper);
	static std::vector<UserCollectionInfo> loadCollectionList(const char* sUsername, const char* sApiKey);

	UserCollectionSettings settings;
	
private:
	static constexpr int s_nPerPage = 24;
	CollectionManager* m_pCollectionManager = nullptr;
	char m_sCollectionUrl[255] = "";
};