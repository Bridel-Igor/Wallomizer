#include "CollectionManager.h"

#include <ctime>

#include "AppState.h"
#include "WinUtils.h"
#include "Settings.h"
#include "WallpaperManager.h"
#include "IUI.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "BinaryIO.h"
#include "Player.h"

CollectionManager::CollectionManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager, const IUI& ui) :
	m_appState(appState),
	m_winUtils(winUtils),
	m_settings(settings),
	m_wallpaperManager(wallpaperManager),
	m_ui(ui),
	m_randomGenerator(static_cast<std::minstd_rand::result_type>(time(nullptr)))
{
	if (!loadSettings())
		m_appState.setFirstLaunch();
	recountWallpapers();
}

CollectionManager::~CollectionManager() = default;

bool CollectionManager::saveSettings() const
{
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"CollectionManager.dat";
	const std::uint32_t collectionsCount = static_cast<std::uint32_t>(m_collections.size());

	BinaryWriter file(filePath);
	if (!file.isOpen()
		|| !file.write(FILE_VERSION)
		|| !file.write(collectionsCount))
		return false;

	for (const auto& collection : m_collections)
		if (!collection->saveSettings(file))
			return false;

	return true;
}

bool CollectionManager::loadSettings()
{
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"CollectionManager.dat";

	BinaryReader file(filePath);
	std::uint16_t fileVersion;
	if (!file.isOpen()
		|| !file.read(fileVersion))
		return false;

	m_collections.clear();

	switch (fileVersion)
	{
	case 4U:
	case 3U:
	{
		std::uint32_t collectionCount;
		if (!file.read(collectionCount))
			return false;

		for (std::uint32_t i = 0; i < collectionCount; i++)
		{
			CollectionType collectionType;
			if (!file.read(collectionType))
				return false;

			std::unique_ptr<BaseCollection> collection;
			switch (collectionType)
			{
			case CollectionType::local:
				collection = std::make_unique<LocalCollection>(m_winUtils, *this);
				break;

			case CollectionType::user:
				collection = std::make_unique<UserCollection>(m_settings);
				break;

			case CollectionType::search:
				collection = std::make_unique<SearchCollection>(m_settings, *this);
				break;

			default:
				return false;
			}

			if (collection->loadSettings(file, fileVersion))
			{
				collection->update();
				m_collections.push_back(std::move(collection));
			}
			else
				return false;

		}
		break;
	}
	default:
	{
		MessageBox(nullptr, "Incompatible collection list file. Collection list was reset.", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
		break;
	}
	}

	if (fileVersion != FILE_VERSION)
		saveSettings();

	return true;
}

void CollectionManager::recountWallpapers()
{
	const std::size_t prevWallpaperCount = m_wallpaperCount;
	m_wallpaperCount = 0;
	
	for (std::size_t i = 0; i < m_collections.size(); i++)
		m_wallpaperCount += m_collections[i]->isEnabled() * static_cast<std::uint32_t>(m_collections[i]->getWallpaperCount());

	m_appState.setNoWallpapers(m_wallpaperCount == 0);

	if (prevWallpaperCount != m_wallpaperCount)
	{
		m_ui.requestCollectionListUpdate();
		m_lastRandomIndex = std::numeric_limits<std::size_t>::max();
	}
}

void CollectionManager::addCollection(std::unique_ptr<BaseCollection> collection)
{
	if (!collection)
		return;
	m_collections.push_back(std::move(collection));
	saveSettings();
	recountWallpapers();
	m_wallpaperManager.onCollectionsChanged();
	if (m_collections.size() == 1)
		m_wallpaperManager.play();
}

void CollectionManager::eraseCollection(std::size_t index)
{
	if (index >= m_collections.size())
		return;
	m_collections.erase(m_collections.begin() + index);
	saveSettings();
	recountWallpapers();
	m_wallpaperManager.onCollectionsChanged();
}

void CollectionManager::enableCollection(std::size_t index, bool enabled)
{
	if (index >= m_collections.size())
		return;
	m_collections.at(index)->enableCollection(enabled);
	m_collections.at(index)->update();
	saveSettings();
	recountWallpapers();
	m_wallpaperManager.onCollectionsChanged();
	if (enabled)
	{
		std::size_t enabledCount = 0;
		for (const auto& collection : m_collections)
			if (collection->isEnabled())
				enabledCount++;
		if (enabledCount == 1)
			m_wallpaperManager.play();
	}
}

Wallpaper CollectionManager::getWallpaper(std::size_t index) const
{
	for (const auto& collection : m_collections)
	{
		if (!collection || !collection->isEnabled())
			continue;
		const std::size_t count = collection->getWallpaperCount();
		if (index < count)
			return collection->getWallpaper(index);
		index -= count;
	}
	return Wallpaper::getEmptyWallpaper();
}

Wallpaper CollectionManager::getRandomWallpaper() const
{
	if (m_wallpaperCount == 0)
		return Wallpaper::getEmptyWallpaper();

	std::size_t index = 0;
	std::uniform_int_distribution<std::size_t> distribution(0, m_wallpaperCount - 1);
	do
	{
		index = distribution(m_randomGenerator);
	} while (m_wallpaperCount>1 && index == m_lastRandomIndex);

	m_lastRandomIndex = index;

	return getWallpaper(index);
}
