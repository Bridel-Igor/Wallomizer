#pragma once

#include <mutex>

class WinUtils;
class Settings;
class WallpaperManager;

class Timer
{
public:
	enum class Status : unsigned char
	{
		paused,
		playing,
		stopped
	};

public:
	class LoadingGuard
	{
	private:
		friend class Timer;
		explicit LoadingGuard(Timer& timer) noexcept :
			m_timer(timer)
		{
			m_timer.m_loading = true;
		}
	public:
		~LoadingGuard() noexcept
		{
			m_timer.m_loading = false;
		}
	private:
		Timer& m_timer;
	};

public:
	explicit Timer(const WinUtils& winUtils, Settings& settings, WallpaperManager& wallpaperManager);
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	bool saveSession();
	bool loadSession();
	void run();

	void play() noexcept;
	void pause() noexcept;
	void stop() noexcept;
	void abort() noexcept { m_abort = true; }
	void repeat() noexcept { m_repeat = true; }
	Status getStatus() const noexcept { return m_status; }
	unsigned long getRemainingTime() const noexcept;
	LoadingGuard loadingGuard() { return LoadingGuard(*this); }
	bool isLoading() const noexcept { return m_loading; }

private:
	const WinUtils& m_winUtils;
	Settings& m_settings;
	WallpaperManager& m_wallpaperManager;

	Status m_status = Status::playing;
	std::mutex m_sessionFileAccess;

	bool m_abort = false;
	bool m_repeat = false;
	unsigned long m_timePassed = 0;
	bool m_loading = false;
};
