#include "Timer.h"

#include <windows.h>

#include "App.h"
#include "Player.h"
#include "Wallpaper.h"

Timer::Timer(App& app) :
	m_app(app)
{
	loadSession(m_app.getCollectionManager().pCurrent);
}

void Timer::saveSession(const Wallpaper *pCurrent)
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	wchar_t wsPath[MAX_PATH];
	wcscpy_s(wsPath, MAX_PATH, m_app.getWinUtils().getRoamingDir());
	wcscat_s(wsPath, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wsPath, L"wb");
	if (pFile == NULL)
		return;
	fwrite(&m_status, sizeof(m_status), 1, pFile);
	fwrite(&m_timePassed, sizeof(m_timePassed), 1, pFile);
	Collection::Type type = Collection::Type::none;
	const wchar_t* path = nullptr;
	if (pCurrent)
	{
		type = pCurrent->getType();
		path = pCurrent->getPath().c_str();
	}
	fwrite(&type, sizeof(Collection::Type), 1, pFile);
	fwrite(path, sizeof(wchar_t), Collection::getMaxPathSize(type), pFile);
	fclose(pFile);
}

void Timer::loadSession(Wallpaper*& pCurrent)
{
	std::lock_guard<std::mutex> lock(m_sessionFileAccess);
	wchar_t wsPath[MAX_PATH];
	wcscpy_s(wsPath, MAX_PATH, m_app.getWinUtils().getRoamingDir());
	wcscat_s(wsPath, MAX_PATH, L"Session.dat\0");
	FILE* pFile;
	_wfopen_s(&pFile, wsPath, L"rb");
	if (pFile == NULL)
		return;
	fread(&m_status, sizeof(m_status), 1, pFile);
	fread(&m_timePassed, sizeof(m_timePassed), 1, pFile);
	if (pCurrent == nullptr)
	{
		Collection::Type type;
		fread(&type, sizeof(Collection::Type), 1, pFile);
		wchar_t* wsWallpaperPath = new wchar_t[getMaxPathSize(type) + 1] {};
		fread(wsWallpaperPath, sizeof(wchar_t), getMaxPathSize(type), pFile);
		pCurrent = new Wallpaper(type, wsWallpaperPath);
		delete[] wsWallpaperPath;
	}
	fclose(pFile);
	DeleteFileW(wsPath);
}

void Timer::run()
{
	while (m_timePassed < m_app.getSettings().delay)
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
			Player::updateTimer(m_app);
			m_timePassed += 100;
		}
	}
	m_timePassed = 0;
}

void Timer::play() noexcept
{
	m_status = Status::playing;
	m_app.getWinUtils().updateDesktopBackground(true);
}

void Timer::pause() noexcept
{
	m_status = Status::paused;
	m_app.getWinUtils().updateDesktopBackground(true);
	saveSession(m_app.getCollectionManager().pCurrent);
}

void Timer::stop() noexcept
{
	m_status = Status::stopped;
	m_app.getWinUtils().updateDesktopBackground(false);
	saveSession(m_app.getCollectionManager().pCurrent);
}

unsigned long Timer::getRemainingTime() const noexcept
{
	const unsigned long delay = m_app.getSettings().delay;
	return delay > m_timePassed ? 
			delay - m_timePassed : 
			0;
}
