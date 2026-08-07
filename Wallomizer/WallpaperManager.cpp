#include "WallpaperManager.h"

#include "App.h"
#include "Player.h"

WallpaperManager::WallpaperManager(App& app) :
	m_app(app),
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
	m_NextWallpaper = std::move(m_app.getCollectionManager().getRandomWallpaper());
	m_NextWallpaper.loadWallpaper(m_app.getWinUtils());
}

void WallpaperManager::setLoadedWallpaper(bool setPrevious)
{
	std::lock_guard<std::mutex> lock(m_imageModification);

	std::filesystem::path pathOfLoaded = m_app.getWinUtils().getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::path pathOfCurrent = m_app.getWinUtils().getRoamingDir() / L"Current wallpaper.jpg";

	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_app.getTimer().abort();
		return;
	}

	if (!setPrevious)
	{
		m_wallpaperList.push_back(m_NextWallpaper);
		if (m_wallpaperList.size() > m_app.getSettings().getData().prevCount + 1)
			m_wallpaperList.pop_front();
	}

	std::error_code ec;

	std::filesystem::remove(pathOfCurrent, ec);
	if (ec)
		return;

	std::filesystem::rename(pathOfLoaded, pathOfCurrent, ec);
	if (ec)
		return;

	m_app.getWinUtils().updateDesktopBackground(m_app.getTimer().getStatus() != Timer::Status::stopped);
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
	m_wallpaperList.back().loadWallpaper(m_app.getWinUtils());
	setLoadedWallpaper(true);
	loadNextWallpaper();
	
	Player::redrawPlayers();
}

void WallpaperManager::deleteLoaded()
{
	std::lock_guard<std::mutex> lock(m_imageModification);

	std::filesystem::path pathOfLoaded = m_app.getWinUtils().getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::remove(pathOfLoaded);
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	if (!m_wallpaperList.empty())
		m_wallpaperList.back().openExternally();
}
