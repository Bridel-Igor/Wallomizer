#pragma once

#include <cstdint>
#include <atomic>

class Settings;

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
	struct State
	{
		Status status;
		std::uint32_t timePassed;
	};

	State getState() const noexcept;
	void setState(State state) noexcept;

public:
	explicit Timer(const Settings& settings) :
		m_settings(settings)
	{}

	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(Timer&&) = delete;

	void run();

	void play() noexcept { m_status.store(Status::playing, std::memory_order_relaxed); }
	void pause() noexcept { m_status.store(Status::paused, std::memory_order_relaxed); }
	void stop() noexcept { m_status.store(Status::stopped, std::memory_order_relaxed); cancel(); }
	void cancel() noexcept { m_cancel.store(true, std::memory_order_relaxed); }

	bool isStopped() noexcept { return m_status.load(std::memory_order_relaxed) == Status::stopped; }
	Status getStatus() const noexcept { return m_status.load(std::memory_order_relaxed); }
	std::uint32_t getRemainingTime() const noexcept;

private:
	const Settings& m_settings;

	std::atomic<Status> m_status = Status::stopped;
	std::atomic_bool m_cancel = false;
	std::atomic<std::uint32_t> m_timePassed = 0;
};
