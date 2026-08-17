#pragma once

#include <mutex>
#include <atomic>

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
	explicit Timer(const WinUtils& winUtils, const Settings& settings, WallpaperManager& wallpaperManager);
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
	void cancel() noexcept { m_cancel = true; }
	Status getStatus() const noexcept { return m_status; }
	std::uint32_t getRemainingTime() const noexcept;
	LoadingGuard loadingGuard() { return LoadingGuard(*this); }
	bool isLoading() const noexcept { return m_loading; }

private:
	const WinUtils& m_winUtils;
	const Settings& m_settings;
	WallpaperManager& m_wallpaperManager;

	std::mutex m_sessionFileAccess;
	std::atomic<Status> m_status = Status::playing;
	std::atomic_bool m_cancel = false;
	std::atomic<std::uint32_t> m_timePassed = 0;
	std::atomic_bool m_loading = false;
};
