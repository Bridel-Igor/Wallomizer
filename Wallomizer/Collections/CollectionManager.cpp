#include "CollectionManager.h"

#include <ctime>
#include <Windows.h>

#include "AppState.h"
#include "WinUtils.h"
#include "Settings.h"
#include "WallpaperManager.h"
#include "IUI.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "BinaryIO.h"
#include "Exceptions.h"

CollectionManager::CollectionManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager, const IUI& ui) :
	m_appState(appState),
	m_winUtils(winUtils),
	m_settings(settings),
	m_wallpaperManager(wallpaperManager),
	m_ui(ui),
	m_randomGenerator(static_cast<std::minstd_rand::result_type>(time(nullptr)))
{
	switch (loadSettings())
	{
	case LoadResult::success:
		break;

	case LoadResult::failed:
	{
		const int result = MessageBoxW(nullptr,
			L"The collection list file is corrupted or incompatible with this version of Wallomizer.\n\n"
			L"Reset the collection list and continue?",
			L"Wallomizer", MB_OKCANCEL | MB_ICONWARNING);

		if (result == IDCANCEL)
			throw StartupCancelled{};

		m_collections.clear();
		saveSettings();
		m_appState.setFirstLaunch();
		break;
	}

	case LoadResult::noFile:
		m_collections.clear();
		saveSettings();
		m_appState.setFirstLaunch();
		break;

	case LoadResult::needsUpdate:
		saveSettings();
		break;
	}

	recountWallpapers();
}

bool CollectionManager::saveSettings() const
{
	const std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"CollectionManager.dat";
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

CollectionManager::LoadResult CollectionManager::loadSettings()
{
	const std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"CollectionManager.dat";

	if(!std::filesystem::exists(filePath))
		return LoadResult::noFile;

	std::uint16_t fileVersion = 0;
	BinaryReader file(filePath);
	if (!file.isOpen()
		|| !file.read(fileVersion))
		return LoadResult::failed;

	m_collections.clear();

	switch (fileVersion)
	{
	case 4U:
	case 3U:
	{
		std::uint32_t collectionCount;
		if (!file.read(collectionCount))
			return LoadResult::failed;

		for (std::uint32_t i = 0; i < collectionCount; i++)
		{
			CollectionType collectionType;
			if (!file.read(collectionType))
				return LoadResult::failed;

			std::unique_ptr<BaseCollection> collection;
			switch (collectionType)
			{
			case CollectionType::local:
				collection = std::make_unique<LocalCollection>(*this, m_winUtils);
				break;

			case CollectionType::user:
				collection = std::make_unique<UserCollection>(*this, m_settings);
				break;

			case CollectionType::search:
				collection = std::make_unique<SearchCollection>(*this, m_settings);
				break;

			default:
				return LoadResult::failed;
			}

			if (collection->loadSettings(file, fileVersion))
			{
				collection->update();
				m_collections.push_back(std::move(collection));
			}
			else
				return LoadResult::failed;
		}
		break;
	}
	default:
	{
		return LoadResult::failed;
	}
	}

	if (fileVersion != FILE_VERSION)
		return LoadResult::needsUpdate;

	return LoadResult::success;
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
		m_lastRandomIndex = std::numeric_limits<std::size_t>::max();
		if (!m_appState.isStarting())
			m_ui.requestCollectionListUpdate();
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
