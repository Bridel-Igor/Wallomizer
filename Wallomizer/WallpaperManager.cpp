#include "WallpaperManager.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

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

	std::wstring pathOfLoaded = m_app.getWinUtils().getRoamingDir();
	pathOfLoaded += L"Loaded wallpaper.dat";
	std::wstring pathOfCurrent = m_app.getWinUtils().getRoamingDir();
	pathOfCurrent += L"Current wallpaper.jpg";

	if (!std::experimental::filesystem::exists(pathOfLoaded))
	{
		m_app.getTimer().abort();
		return;
	}
	if (!setPrevious)
	{
		m_wallpaperList.push_back(m_NextWallpaper);
		if (m_wallpaperList.size() > m_app.getSettings().prevCount + 1)
			m_wallpaperList.pop_front();
	}
	DeleteFileW(pathOfCurrent.c_str());
	if (MoveFileW(pathOfLoaded.c_str(), pathOfCurrent.c_str()) == 0)
	{
		return;
	}
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

	std::wstring pathOfLoaded = m_app.getWinUtils().getRoamingDir();
	pathOfLoaded += L"Loaded wallpaper.dat";

	DeleteFileW(pathOfLoaded.c_str());
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	m_wallpaperList.back().openExternally();
}
