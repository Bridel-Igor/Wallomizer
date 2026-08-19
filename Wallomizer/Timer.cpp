#include "Timer.h"

#include <thread>
#include <chrono>

#include "Settings.h"
#include "Player.h"

Timer::State Timer::getState() const noexcept
{
	return {
		m_status.load(std::memory_order_relaxed), 
		m_timePassed.load(std::memory_order_relaxed)
	};
}

void Timer::setState(State state) noexcept
{
	m_status.store(state.status, std::memory_order_relaxed);
	m_timePassed.store(state.timePassed, std::memory_order_relaxed);
}

void Timer::run()
{
	constexpr std::chrono::milliseconds tick = std::chrono::milliseconds(100);
	m_timePassed = 0;
	while (m_timePassed.load(std::memory_order_relaxed) < m_settings.getData().delay)
	{
		if (m_cancel.exchange(false, std::memory_order_relaxed))
			return;
		std::this_thread::sleep_for(tick);
		if (m_status == Status::playing)
		{
			Player::updateTimer();
			m_timePassed.fetch_add(static_cast<std::uint32_t>(tick.count()), std::memory_order_relaxed);
		}
	}
}

std::uint32_t Timer::getRemainingTime() const noexcept
{
	const std::uint32_t delay = m_settings.getData().delay;
	const std::uint32_t timePassed = m_timePassed.load(std::memory_order_relaxed);

	return delay > timePassed ?
			delay - timePassed :
			0;
}
