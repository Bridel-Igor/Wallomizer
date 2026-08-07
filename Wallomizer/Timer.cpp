#include "Timer.h"

#include "WinUtils.h"
#include "Settings.h"
#include "WallpaperManager.h"
#include "Player.h"
#include "BinaryIO.h"

Timer::Timer(const WinUtils& winUtils, Settings& settings, WallpaperManager& wallpaperManager) :
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

	const Wallpaper& wallpaper = m_wallpaperManager.getCurrentWallpaper();

	BinaryWriter file(filePath);
	return file.isOpen()
		&& file.write(m_status)
		&& file.write(m_timePassed)
		&& file.write(wallpaper.getType())
		&& file.write(wallpaper.getPath());
}

bool Timer::loadSession()
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";

	Collection::Type type;
	std::wstring path;

	BinaryReader file(filePath);
	if (!file.isOpen()
		|| !file.read(m_status)
		|| !file.read(m_timePassed)
		|| !file.read(type)
		|| !file.read(path))
		return false;
		
	Wallpaper loadedWallpaper(type, path);
	m_wallpaperManager.setCurrentWallpaper(std::move(loadedWallpaper));
	return true;
}

void Timer::run()
{
	while (m_timePassed < m_settings.getData().delay)
	{
		if (m_abort)
		{
			m_abort = false;
			m_timePassed = 0;
			return;
		}
		if (m_repeat)
		{
			m_repeat = false;
			m_timePassed = 0;
			continue;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (m_status == Status::playing)
		{
			Player::updateTimer(*this);
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

unsigned long Timer::getRemainingTime() const noexcept
{
	const unsigned long delay = m_settings.getData().delay;
	return delay > m_timePassed ? 
			delay - m_timePassed : 
			0;
}
