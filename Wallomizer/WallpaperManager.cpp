#include "WallpaperManager.h"

#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Timer.h"
#include "Player.h"

WallpaperManager::WallpaperManager(const WinUtils& winUtils, const Settings& settings, CollectionManager& collectionManager, Timer& timer) :
	m_winUtils(winUtils),
	m_settings(settings),
	m_collectionManager(collectionManager),
	m_timer(timer),
	m_NextWallpaper(Wallpaper::getEmptyWallpaper())
{
}

void WallpaperManager::setCurrentWallpaper(Wallpaper&& wallpaper)
{
	m_wallpaperList.clear();
	m_wallpaperList.push_back(std::move(wallpaper));
}

const Wallpaper WallpaperManager::getCurrentWallpaper() const
{
	if (m_wallpaperList.empty())
		return Wallpaper::getEmptyWallpaper();
	return m_wallpaperList.back();
}

bool WallpaperManager::hasPrevious() const
{
	return m_wallpaperList.size() > 1;
}

void WallpaperManager::loadNextWallpaper()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	m_NextWallpaper = std::move(m_collectionManager.getRandomWallpaper());
	m_NextWallpaper.loadWallpaper(m_winUtils);
}

void WallpaperManager::setLoadedWallpaper(bool setPrevious)
{
	std::lock_guard<std::mutex> lock(m_imageModification);

	std::filesystem::path pathOfLoaded = m_winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::path pathOfCurrent = m_winUtils.getRoamingDir() / L"Current wallpaper.jpg";

	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_timer.abort();
		return;
	}

	if (!setPrevious)
	{
		m_wallpaperList.push_back(m_NextWallpaper);
		if (m_wallpaperList.size() > m_settings.getData().prevCount + 1)
			m_wallpaperList.pop_front();
	}

	std::error_code ec;

	std::filesystem::remove(pathOfCurrent, ec);
	if (ec)
		return;

	std::filesystem::rename(pathOfLoaded, pathOfCurrent, ec);
	if (ec)
		return;

	m_winUtils.updateDesktopBackground(m_timer.getStatus() != Timer::Status::stopped);
	Player::redrawPlayers();
}

void WallpaperManager::setNextWallpaper()
{
	setLoadedWallpaper();
	loadNextWallpaper();
}

void WallpaperManager::setPreviousWallpaper()
{
	if (!hasPrevious())
		return;

	m_wallpaperList.pop_back();
	m_wallpaperList.back().loadWallpaper(m_winUtils);
	setLoadedWallpaper(true);
	loadNextWallpaper();
	
	Player::redrawPlayers();
}

void WallpaperManager::deleteLoaded()
{
	std::lock_guard<std::mutex> lock(m_imageModification);

	std::filesystem::path pathOfLoaded = m_winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::remove(pathOfLoaded);
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	if (!m_wallpaperList.empty())
		m_wallpaperList.back().openExternally();
}
