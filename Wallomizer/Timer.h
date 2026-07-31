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
	Timer(App& app);

	void saveSession(Wallpaper* pCurrent = nullptr);
	void loadSession(Wallpaper*& pCurrent);
	void run();

	void play() noexcept;
	void pause() noexcept;
	void stop() noexcept;
	void abort() noexcept { m_abort = true; }
	void repeat() noexcept { m_repeat = true; }
	const Status getStatus() const noexcept { return m_status; }
	unsigned long getRemainingTime() const;

private:
	App& m_app;
	Status m_status = Timer::Status::playing;
	std::mutex m_sessionFileAccess;

	bool m_abort = false;
	bool m_repeat = false;
	unsigned long m_timePassed = 0;
};
