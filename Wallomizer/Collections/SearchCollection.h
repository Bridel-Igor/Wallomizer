#pragma once

#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"
#include "CollectionManager.h"

class SearchCollection : public BaseCollection
{
private:
	struct SearchCollectionSettings
	{
		CategoriesAndPurity categoriesAndPurity = CAP::categoryGeneral | CAP::categoryAnime | CAP::categoryPeople | CAP::puritySFW;
		char sTag[255] = "";
		char sResolution[255] = "";
		char sRatio[128] = "";
		char sColor[16] = "";
	};

public:
	SearchCollection(CollectionManager* collectionManager) :
		m_pCollectionManager(collectionManager)
	{}

	~SearchCollection() {};
	bool saveSettings(FILE* pFile) const;
	bool loadSettings(FILE* pFile);
	void getCollectionName(wchar_t* pwsName, size_t size) const;
	CollectionType getCollectionType() const { return CollectionType::search; }
	CategoriesAndPurity getCAP() const;
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND hCaller);

	static bool loadWallpaper(const Wallpaper* pWallpaper);
	static void openWallpaperExternal(const Wallpaper* pWallpaper);

	SearchCollectionSettings settings;

private:
	static constexpr int s_nPerPage = 24;
	CollectionManager* m_pCollectionManager = nullptr;
	char m_sSearchUrl[1024] = "";	
};