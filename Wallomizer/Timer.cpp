#include "Timer.h"

#include <windows.h>
#include <string>

#include "WinUtils.h"
#include "Settings.h"
#include "WallpaperManager.h"
#include "Player.h"
#include "Wallpaper.h"

Timer::Timer(const WinUtils& winUtils, Settings& settings, WallpaperManager& wallpaperManager) :
	m_winUtils(winUtils),
	m_settings(settings),
	m_wallpaperManager(wallpaperManager)
{
	loadSession();
}

void Timer::saveSession()
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";
	FILE* pFile;
	_wfopen_s(&pFile, filePath.c_str(), L"wb");
	if (pFile == nullptr)
		return;
	fwrite(&m_status, sizeof(m_status), 1, pFile);
	fwrite(&m_timePassed, sizeof(m_timePassed), 1, pFile);

	const Wallpaper& wallpaper = m_wallpaperManager.getCurrentWallpaper();
	const uint16_t pathLength = static_cast<uint16_t>(wallpaper.getPath().size());
	const Collection::Type type = wallpaper.getType();
	fwrite(&type, sizeof(type), 1, pFile);
	fwrite(&pathLength, sizeof(pathLength), 1, pFile);
	fwrite(wallpaper.getPath().c_str(), sizeof(wchar_t), pathLength, pFile);

	fclose(pFile);
}

void Timer::loadSession()
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	std::filesystem::path filePath = m_winUtils.getRoamingDir() / L"Session.dat";
	FILE* pFile;
	_wfopen_s(&pFile, filePath.c_str(), L"rb");
	if (pFile == nullptr)
		return;

	fread(&m_status, sizeof(m_status), 1, pFile);
	fread(&m_timePassed, sizeof(m_timePassed), 1, pFile);

	Collection::Type type;
	fread(&type, sizeof(type), 1, pFile);

	uint16_t pathLength;
	fread(&pathLength, sizeof(pathLength), 1, pFile);

	std::wstring path(pathLength, L'\0');
	fread(path.data(), sizeof(wchar_t), pathLength, pFile);

	fclose(pFile);

	Wallpaper loadedWallpaper(type, path.c_str());
	m_wallpaperManager.setCurrentWallpaper(std::move(loadedWallpaper));

	DeleteFileW(filePath.c_str()); // TODO: do i need to delete it?
}

void Timer::run()
{
	while (m_timePassed < m_settings.delay)
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
	const unsigned long delay = m_settings.delay;
	return delay > m_timePassed ? 
			delay - m_timePassed : 
			0;
}
