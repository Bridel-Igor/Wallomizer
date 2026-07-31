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

void Timer::saveSession(Wallpaper *pCurrent)
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
	CollectionType type = CollectionType::none;
	if (pCurrent)
		type = pCurrent->getType();
	fwrite(&type, sizeof(CollectionType), 1, pFile);
	switch (type)
	{
	case CollectionType::local:		fwrite(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
	case CollectionType::user:		fwrite(pCurrent->getPathW(), sizeof(wchar_t), 255, pFile);		break;
	case CollectionType::search:	fwrite(pCurrent->getPathW(), sizeof(wchar_t), 1024, pFile);		break;
	}
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
		CollectionType type;
		fread(&type, sizeof(CollectionType), 1, pFile);
		pCurrent = new Wallpaper(type);
		switch (type)
		{
		case CollectionType::local:		fread(pCurrent->getPathW(), sizeof(wchar_t), MAX_PATH, pFile);	break;
		case CollectionType::user:		fread(pCurrent->getPathW(), sizeof(wchar_t), 255, pFile);		break;
		case CollectionType::search:	fread(pCurrent->getPathW(), sizeof(wchar_t), 1024, pFile);		break;
		}
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

unsigned long Timer::getRemainingTime() const
{
	return m_app.getSettings().delay > m_timePassed ? m_app.getSettings().delay - m_timePassed : 0;
}
