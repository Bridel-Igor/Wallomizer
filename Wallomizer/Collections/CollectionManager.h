#pragma once

#include <vector>
#include <random>

#include "CollectionTypes.h"

class App;
class Wallpaper;
class BaseCollection;

class CollectionManager
{
public:
	CollectionManager(App& app);
	~CollectionManager();

	bool saveSettings() const;
	bool loadSettings();
	Wallpaper getWallpaper(std::uint32_t index) const;
	void openMainWindow();

	void reloadSettings();
	void clear();
	void updateNumber();
	void addCollection(Collection::Type collectionType);
	void eraseCollection(std::size_t index);
	Wallpaper getRandomWallpaper();
	std::uint32_t getNumber() const noexcept{ return m_number; }
	
	std::vector<BaseCollection*> m_pCollections;

private:
	static constexpr std::uint16_t FILE_VERSION = 4U;

	App& m_app;

	std::uint32_t m_number = 0;
	std::mt19937 m_randomGenerator;
	std::uniform_int_distribution<int> m_uniformIntDistribution;
};
