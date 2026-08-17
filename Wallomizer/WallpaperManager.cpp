#include "WallpaperManager.h"

#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Timer.h"
#include "Player.h"

WallpaperManager::WallpaperManager(const WinUtils& winUtils, const Settings& settings, const CollectionManager& collectionManager, Timer& timer) :
	m_winUtils(winUtils),
	m_settings(settings),
	m_collectionManager(collectionManager),
	m_timer(timer),
	pathOfLoaded(m_winUtils.getRoamingDir() / L"Loaded wallpaper.dat"),
	pathOfCurrent(m_winUtils.getRoamingDir() / L"Current wallpaper.jpg"),
	m_nextWallpaper(Wallpaper::getEmptyWallpaper())
{
}

void WallpaperManager::setCurrentWallpaper(Wallpaper&& wallpaper)
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	m_wallpaperList.clear();
	m_wallpaperList.push_back(std::move(wallpaper));
}

const Wallpaper WallpaperManager::getCurrentWallpaper() const
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	if (m_wallpaperList.empty())
		return Wallpaper::getEmptyWallpaper();
	return m_wallpaperList.back();
}

bool WallpaperManager::hasCurrent() const
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	return !m_wallpaperList.empty();
}

bool WallpaperManager::hasPrevious() const
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	return m_wallpaperList.size() > 1;
}

void WallpaperManager::loadImage()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_nextWallpaper = std::move(m_collectionManager.getRandomWallpaper());
		m_nextWallpaper.loadWallpaper(m_winUtils);
	}
}

void WallpaperManager::setLoadedWallpaper()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_timer.cancel();
		return;
	}
	if (!m_settingPrevious)
	{
		m_wallpaperList.push_back(m_nextWallpaper);
		if (m_wallpaperList.size() > m_settings.getData().prevCount + 1)
			m_wallpaperList.pop_front();
	}
	m_settingPrevious = false;

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

void WallpaperManager::nextWallpaper()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	m_timer.cancel();
}

void WallpaperManager::previousWallpaper()
{
	std::lock_guard<std::mutex> lock(m_imageModification);

	if (m_wallpaperList.size() <= 1)
		return;

	m_wallpaperList.pop_back();
	m_wallpaperList.back().loadWallpaper(m_winUtils);
	m_settingPrevious = true;
	m_timer.cancel();
}

void WallpaperManager::deleteLoaded()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	std::filesystem::remove(pathOfLoaded);
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	if (!m_wallpaperList.empty())
		m_wallpaperList.back().openExternally();
}
