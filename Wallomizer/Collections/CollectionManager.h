#pragma once

#include <list>
#include <vector>
#include <random>
#include <mutex>

#include "BaseCollection.h"

class CollectionManager : public BaseCollection
{
public:
	CollectionManager();

	~CollectionManager();
	bool saveSettings(FILE* pFile = nullptr) const;
	bool loadSettings(FILE* pFile = nullptr);
	void getCollectionName(wchar_t*, size_t) const {}
	CollectionType getCollectionType() const { return CollectionType::none; }
	CategoriesAndPurity getCAP() const { return 0; }
	Wallpaper* getWallpaperInfo(unsigned int index) const;
	void openCollectionSettingsWindow(HWND = nullptr);

	void reloadSettings();
	void clear();
	void updateNumber();
	void addCollection(CollectionType collectionType);
	void eraseCollection(int index);
	void loadNextWallpaper();
	void loadRandomWallpaper();
	void setLoadedWallpaper(bool setPrevious = false);
	void setNextWallpaper();
	void setPreviousWallpaper();
	void openWallpaperExternal() const;
	bool hasPrevious() const;
	bool isReady() const { return m_isReady; }

	static bool loadWallpaper(const Wallpaper *pWallpaper);
	
	std::vector<BaseCollection*> m_pCollections;
	bool m_isLoading = false;
	Wallpaper *pCurrent = nullptr, *pNext = nullptr;
	
private:
	std::list<Wallpaper*> pPreviousList;
	bool m_isReady = false;
	std::mt19937 m_randomGenerator;
	std::uniform_int_distribution<int> m_uniformIntDistribution;
	std::mutex imageModification;
};