#include "Timer.h"

#include "WinUtils.h"
#include "Settings.h"
#include "WallpaperManager.h"
#include "Player.h"
#include "BinaryIO.h"

Timer::Timer(const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager) :
	m_winUtils(winUtils),
	m_settings(settings),
	m_wallpaperManager(wallpaperManager)
{
	loadSession();
}

bool Timer::saveSession()
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";

	const Timer::Status status = m_status;
	const std::uint32_t timePassed = m_timePassed;
	const Wallpaper& wallpaper = m_wallpaperManager.getCurrentWallpaper();

	BinaryWriter file(filePath);
	return file.isOpen()
		&& file.write(status)
		&& file.write(timePassed)
		&& file.write(wallpaper.getType())
		&& file.write(wallpaper.getPath());
}

bool Timer::loadSession()
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
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
		
	m_status = status;
	m_timePassed = timePassed;
	Wallpaper loadedWallpaper(type, path);
	m_wallpaperManager.setCurrentWallpaper(std::move(loadedWallpaper));
	return true;
}

void Timer::run()
{
	while (m_timePassed < m_settings.getData().delay)
	{
		if (m_cancel)
		{
			m_cancel = false;
			m_timePassed = 0;
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (m_status == Status::playing)
		{
			Player::updateTimer();
			m_timePassed += 100;
		}
	}
	m_timePassed = 0;
}

void Timer::play() noexcept
{
	m_status = Status::playing;
	m_winUtils.updateDesktopBackground(true);
}

void Timer::pause() noexcept
{
	m_status = Status::paused;
	m_winUtils.updateDesktopBackground(true);
	saveSession();
}

void Timer::stop() noexcept
{
	m_status = Status::stopped;
	m_winUtils.updateDesktopBackground(false);
	saveSession();
}

std::uint32_t Timer::getRemainingTime() const noexcept
{
	const std::uint32_t delay = m_settings.getData().delay;
	return delay > m_timePassed ? 
			delay - m_timePassed : 
			0;
}
