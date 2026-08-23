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
	m_pathOfLoaded(m_winUtils.getRoamingDir() / L"Loaded wallpaper.dat"),
	m_pathOfCurrent(m_winUtils.getRoamingDir() / L"Current wallpaper.jpg"),
	m_nextWallpaper(Wallpaper::getEmptyWallpaper())
{
	loadSession();
	onCollectionsChanged();
}

// Session

bool WallpaperManager::saveSession()
{
	std::lock_guard<std::mutex> sessionFileLock(m_sessionFileAccess);
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
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";

	Timer::State timerState{};
	CollectionType type;
	std::wstring path;

	BinaryReader file(filePath);
	if (!file.isOpen()
		|| !file.read(timerState.status)
		|| !file.read(timerState.timePassed)
		|| !file.read(type)
		|| !file.read(path))
		return false;

	m_timer.setState(timerState);

	m_wallpaperList.clear();
	if (type != CollectionType::none)
		m_wallpaperList.emplace_back(type, path);
	return true;
}

void WallpaperManager::clear()
{
	m_timer.stop();
	m_winUtils.updateDesktopBackground(false);

	removeLoadedFile();
	removeCurrentFile();

	m_wallpaperList.clear();
	m_nextWallpaper = Wallpaper::getEmptyWallpaper();
	m_isLoadedImageReady = false;

	saveSession();
}

// Lifecycle

void WallpaperManager::loadImage()
{
	m_isLoadedImageReady = false;
	Player::redrawPlayers();

	std::unique_lock lock(m_imageModification);

	if (!removeLoadedFile())
	{
		clear();
		return;
	}

	if (m_nextWallpaper.isValid())
		m_timer.cancel();
	else
		m_nextWallpaper = std::move(m_collectionManager.getRandomWallpaper());

	m_isLoadedImageReady = m_nextWallpaper.loadWallpaper(m_winUtils);

	// failed to load. Going to the next cycle to load the new one. 
	if (!m_isLoadedImageReady)
	{
		removeLoadedFile();
		m_nextWallpaper = Wallpaper::getEmptyWallpaper();
		m_timer.cancelKeepTime();
	}

	lock.unlock();
	Player::redrawPlayers();
}

void WallpaperManager::applyLoadedImage()
{
	std::lock_guard lock(m_imageModification);

	if (!hasLoadedFile()
		|| !m_isLoadedImageReady
		|| !m_nextWallpaper.isValid())
		return;

	m_isLoadedImageReady = false;
	Player::redrawPlayers();

	if (!makeLoadedFileCurrent())
	{
		clear();
		return;
	}

	m_wallpaperList.push_back(m_nextWallpaper);
	m_nextWallpaper = Wallpaper::getEmptyWallpaper();
	if (m_wallpaperList.size() > m_settings.getData().prevCount + 1)
		m_wallpaperList.pop_front();

	m_winUtils.updateDesktopBackground(true);
	saveSession();
}

void WallpaperManager::onCollectionsChanged()
{
	std::lock_guard lock(m_imageModification);

	if (m_appState.isNoWallpapers())
	{
		clear();
		return;
	}

	if (!m_appState.isStarting())
		m_timer.cancelKeepTime();

	m_nextWallpaper = Wallpaper::getEmptyWallpaper();
	m_isLoadedImageReady = false;
	removeLoadedFile();
	Player::redrawPlayers();
}

// Controls

void WallpaperManager::previousWallpaper()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canPrevious())
		return;

	m_wallpaperList.pop_back();
	m_nextWallpaper = std::move(m_wallpaperList.back());
	m_wallpaperList.pop_back();
	removeLoadedFile();
	m_isLoadedImageReady = false;
	m_timer.cancel();
}

void WallpaperManager::openCurrentWallpaperExternally()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canOpen())
		return;

	m_wallpaperList.back().openExternally(m_winUtils);
}

void WallpaperManager::stop()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canStop())
		return;

	clear();
}

void WallpaperManager::play()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canPlay())
		return;

	m_timer.play();
	if (m_wallpaperList.empty())
		m_timer.cancel();
	m_winUtils.updateDesktopBackground(true);

	saveSession();
}

void WallpaperManager::pause()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canPause())
		return;

	m_timer.pause();
	m_winUtils.updateDesktopBackground(true);
	saveSession();
}

void WallpaperManager::fit()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canFit())
		return;

	m_winUtils.flipWallpaperStyle();
}

void WallpaperManager::nextWallpaper()
{
	std::unique_lock lock(m_imageModification, std::try_to_lock);
	if (!lock.owns_lock() || !canNext())
		return;

	m_timer.cancel();
}

// Checks

bool WallpaperManager::canPrevious() const noexcept
{
	return !m_appState.isLoading() 
		&& !m_timer.isStopped() 
		&& hasPrevious() 
		&& m_isLoadedImageReady;
}

bool WallpaperManager::canOpen() const noexcept
{
	return !m_appState.isLoading() 
		&& !m_timer.isStopped() 
		&& hasCurrent();
}

bool WallpaperManager::canStop() const noexcept
{
	return !m_timer.isStopped() 
		&& m_isLoadedImageReady
		&& !m_appState.isLoading();
}

bool WallpaperManager::canPlay() const noexcept
{
	return !m_appState.isLoading() 
		&& !m_timer.isPlaying() 
		&& !m_appState.isNoWallpapers();
}

bool WallpaperManager::canPause() const noexcept
{
	return !m_appState.isLoading() 
		&& m_timer.isPlaying() 
		&& !m_appState.isNoWallpapers();
}

bool WallpaperManager::canFit() const noexcept
{
	return !m_appState.isLoading() 
		&& hasCurrent() 
		&& !m_timer.isStopped();
}

bool WallpaperManager::canNext() const noexcept
{
	return !m_appState.isLoading() 
		&& !m_appState.isNoWallpapers() 
		&& !m_timer.isStopped() 
		&& m_isLoadedImageReady;
}

// Files

bool WallpaperManager::removeLoadedFile() noexcept
{
	std::error_code ec;
	std::filesystem::remove(m_pathOfLoaded, ec);
	if (ec)
	{
		m_appState.setFileError(true);
		return false;
	}
	return true;
}

bool WallpaperManager::removeCurrentFile() noexcept
{
	std::error_code ec;
	std::filesystem::remove(m_pathOfCurrent, ec);
	if (ec)
	{
		m_appState.setFileError(true);
		return false;
	}
	return true;
}

bool WallpaperManager::makeLoadedFileCurrent() noexcept
{
	if (!removeCurrentFile())
		return false;

	std::error_code ec;
	std::filesystem::rename(m_pathOfLoaded, m_pathOfCurrent, ec);
	if (ec)
	{
		m_appState.setFileError(true);
		return false;
	}
	return true;
}

bool WallpaperManager::hasLoadedFile() const noexcept
{
	std::error_code ec;
	const bool result = std::filesystem::exists(m_pathOfLoaded, ec);
	if (ec)
	{
		m_appState.setFileError(true);
		return false;
	}
	return result;
}
