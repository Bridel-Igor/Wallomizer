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
		wchar_t wsTag[255] = L"";
		wchar_t wsResolution[255] = L"";
		wchar_t wsRatio[128] = L"";
		wchar_t wsColor[16] = L"";
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
	wchar_t m_wsSearchUrl[1024] = L"";	
};