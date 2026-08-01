#pragma once

#include <mutex>

class App;
class Wallpaper;

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
	explicit Timer(App& app);
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	void saveSession(const Wallpaper* pCurrent = nullptr);
	void loadSession(Wallpaper*& pCurrent);
	void run();

	void play() noexcept;
	void pause() noexcept;
	void stop() noexcept;
	void abort() noexcept { m_abort = true; }
	void repeat() noexcept { m_repeat = true; }
	Status getStatus() const noexcept { return m_status; }
	unsigned long getRemainingTime() const noexcept;

private:
	App& m_app;
	Status m_status = Status::playing;
	std::mutex m_sessionFileAccess;

	bool m_abort = false;
	bool m_repeat = false;
	unsigned long m_timePassed = 0;
};
