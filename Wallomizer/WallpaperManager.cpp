#include "WallpaperManager.h"

#include "AppState.h"
#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Timer.h"
#include "Player.h"
#include "BinaryIO.h"

WallpaperManager::WallpaperManager(AppState& appState, const WinUtils& winUtils, const Settings& settings, const CollectionManager& collectionManager, Timer& timer) :
	m_appState(appState),
	m_winUtils(winUtils),
	m_settings(settings),
	m_collectionManager(collectionManager),
	m_timer(timer),
	pathOfLoaded(m_winUtils.getRoamingDir() / L"Loaded wallpaper.dat"),
	pathOfCurrent(m_winUtils.getRoamingDir() / L"Current wallpaper.jpg"),
	m_nextWallpaper(Wallpaper::getEmptyWallpaper())
{
	loadSession();
	if (m_collectionManager.getCollectionCount() == 0 || m_timer.isStopped())
		stop();
}

bool WallpaperManager::saveSession()
{
	std::lock_guard<std::mutex> sessionFileLock(m_sessionFileAccess);
	AppState::LoadingGuard loading = m_appState.loadingGuard();
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";

	const Timer::State timerState = m_timer.getState();
	const Wallpaper& wallpaper = m_wallpaperList.empty() ? Wallpaper::getEmptyWallpaper() : m_wallpaperList.back();

	BinaryWriter file(filePath);
	return file.isOpen()
		&& file.write(timerState.status)
		&& file.write(timerState.timePassed)
		&& file.write(wallpaper.getType())
		&& file.write(wallpaper.getPath());
}

bool WallpaperManager::loadSession()
{
	std::lock_guard<std::mutex> sessionFileLock(m_sessionFileAccess);
	AppState::LoadingGuard loading = m_appState.loadingGuard();
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";

	Timer::Status status;
	std::uint32_t timePassed;
	CollectionType type;
	std::wstring path;

	BinaryReader file(filePath);
	if (!file.isOpen()
		|| !file.read(status)
		|| !file.read(timePassed)
		|| !file.read(type)
		|| !file.read(path))
		return false;

	m_timer.setState({ status, timePassed });
	Wallpaper loadedWallpaper(type, path);

	std::lock_guard<std::mutex> imageLock(m_imageModification);
	m_wallpaperList.clear();
	m_wallpaperList.push_back(std::move(loadedWallpaper));
	return true;
}

// Player buttons

void WallpaperManager::previousWallpaper()
{
	if (!canPrevious())
		return;

	std::lock_guard<std::mutex> lock(m_imageModification);
	AppState::LoadingGuard loading = m_appState.loadingGuard();

	m_wallpaperList.pop_back();
	m_wallpaperList.back().loadWallpaper(m_winUtils);
	m_settingPrevious = true;
	m_timer.cancel();
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	if (!canOpen())
		return;

	std::lock_guard<std::mutex> lock(m_imageModification);
	if (!m_wallpaperList.empty())
		m_wallpaperList.back().openExternally(m_winUtils);
}

void WallpaperManager::stop()
{
	if (!canStop())
		return;

	m_winUtils.updateDesktopBackground(false);
	m_timer.stop();
	Player::redrawPlayers();

	std::lock_guard<std::mutex> lock(m_imageModification);

	std::filesystem::remove(pathOfLoaded);
	std::filesystem::remove(pathOfCurrent);

	m_wallpaperList.clear();
	m_nextWallpaper = Wallpaper::getEmptyWallpaper();

	saveSession();
}

void WallpaperManager::play()
{
	if (!canPlay())
		return;

	if (m_collectionManager.getCollectionCount() == 0)
	{
		stop();
		return;
	}

	m_timer.play();
	if (m_wallpaperList.empty())
		m_timer.cancel();
	m_winUtils.updateDesktopBackground(true);
}

void WallpaperManager::pause()
{
	if (!canPause())
		return;

	if (m_collectionManager.getCollectionCount() == 0)
	{
		stop();
		return;
	}
	m_timer.pause();
	m_winUtils.updateDesktopBackground(true);
	saveSession();
}

void WallpaperManager::fit()
{
	if (!canFit())
		return;

	m_winUtils.flipWallpaperStyle();
}

void WallpaperManager::nextWallpaper()
{
	if (!canNext())
		return;

	m_timer.cancel();
}

// End of player buttons

void WallpaperManager::loadImage()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_nextWallpaper = std::move(m_collectionManager.getRandomWallpaper());
		m_nextWallpaper.loadWallpaper(m_winUtils);
	}
}

void WallpaperManager::setLoadedImage()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	AppState::LoadingGuard loading = m_appState.loadingGuard();
	if (!std::filesystem::exists(pathOfLoaded))
	{
		m_timer.cancel();
		return;
	}

	std::error_code ec;
	std::filesystem::remove(pathOfCurrent, ec);
	if (ec)
		return;
	std::filesystem::rename(pathOfLoaded, pathOfCurrent, ec);
	if (ec)
		return;

	if (!m_settingPrevious)
	{
		m_wallpaperList.push_back(m_nextWallpaper);
		if (m_wallpaperList.size() > m_settings.getData().prevCount + 1)
			m_wallpaperList.pop_front();
	}
	m_settingPrevious = false;

	m_winUtils.updateDesktopBackground(true);
	Player::redrawPlayers();
	saveSession();
}

void WallpaperManager::deleteLoadedImage()
{
	std::lock_guard<std::mutex> lock(m_imageModification);
	std::filesystem::remove(pathOfLoaded);
}

bool WallpaperManager::canPrevious() const noexcept
{
	return !m_appState.isLoading() && !m_timer.isStopped() && hasPrevious();
}

bool WallpaperManager::canOpen() const noexcept
{
	return !m_appState.isLoading() && !m_timer.isStopped() && hasCurrent();
}

bool WallpaperManager::canStop() const noexcept
{
	return !m_timer.isStopped();
}

bool WallpaperManager::canPlay() const noexcept
{
	return !m_appState.isLoading() && !m_timer.isPlaying() && !m_appState.isNoWallpapers();
}

bool WallpaperManager::canPause() const noexcept
{
	return !m_appState.isLoading() && m_timer.isPlaying() && !m_appState.isNoWallpapers();
}

bool WallpaperManager::canFit() const noexcept
{
	return !m_appState.isLoading() && hasCurrent() && !m_timer.isStopped();
}

bool WallpaperManager::canNext() const noexcept
{
	return !m_appState.isLoading() && !m_appState.isNoWallpapers() && !m_timer.isStopped();
}
