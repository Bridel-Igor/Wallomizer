#pragma once

#include <list>
#include <vector>
#include <random>
#include <mutex>

#include "BaseCollection.h"

class App;

class CollectionManager : public BaseCollection
{
public:
	CollectionManager(App& app);

	~CollectionManager();
	bool saveSettings(FILE* pFile = nullptr) const;
	bool loadSettings(FILE* pFile = nullptr, unsigned short fileVersion = 0);
	void getCollectionName(wchar_t*, size_t) const {}
	Collection::Type getCollectionType() const { return Collection::Type::none; }
	CategoriesAndPurity getCAP() const { return CAP::none; }
	Wallpaper getWallpaper(unsigned int index) const;
	void openCollectionSettingsWindow(HWND = nullptr);

	void reloadSettings();
	void clear();
	void updateNumber();
	void addCollection(Collection::Type collectionType);
	void eraseCollection(int index);
	Wallpaper getRandomWallpaper();
	
	std::vector<BaseCollection*> m_pCollections;
	bool m_isLoading = false;

	/// Current file version of CollectionManager.dat file.
	static constexpr unsigned short COLLECTION_MANAGER_FILE_VERSION = 3U;
	
private:
	App& m_app;

	std::mt19937 m_randomGenerator;
	std::uniform_int_distribution<int> m_uniformIntDistribution;
};
