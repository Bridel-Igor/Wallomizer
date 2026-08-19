#pragma once

#include <atomic>

#include "Player.h"

class AppState
{
private:
	enum class State
	{
		starting,
		running,
		noWallpapers,
		exiting
	};

public:
	class LoadingGuard
	{
		friend class AppState;

	private:
		explicit LoadingGuard(AppState& appState) noexcept :
			m_appState(appState)
		{
			m_appState.m_loadingCount.fetch_add(1, std::memory_order_relaxed);
			Player::updateTimer();
		}

	public:
		~LoadingGuard() noexcept
		{
			m_appState.m_loadingCount.fetch_sub(1, std::memory_order_relaxed);
			Player::updateTimer();
		}

	private:
		AppState& m_appState;
	};

public:
	void firstLaunch() noexcept			{ m_firstLaunch=true; }
	bool isFirstLaunch() const noexcept	{ return m_firstLaunch; }

	LoadingGuard loadingGuard()		{ return LoadingGuard(*this); }
	bool isLoading() const noexcept	{ return m_loadingCount.load(std::memory_order_relaxed) != 0; }

	void running() noexcept			{ setState(State::running); }
	void noWallpapers() noexcept	{ setState(State::noWallpapers); }
	void exiting() noexcept			{ setState(State::exiting); }

	bool isStarting() const noexcept		{ return getState() == State::starting; }
	bool isRunning() const noexcept			{ return getState() == State::running; }
	bool isNoWallpapers() const noexcept	{ return getState() == State::noWallpapers; }
	bool isExiting() const noexcept			{ return getState() == State::exiting; }

private:
	State getState() const noexcept { return m_state.load(std::memory_order_relaxed); }
	void setState(State state) noexcept
	{
		m_state.store(state, std::memory_order_relaxed);
		Player::updateTimer();
	}

	std::atomic<State> m_state = State::starting;
	std::atomic_uint m_loadingCount = 0;

	// being set and read in the same thread.
	bool m_firstLaunch = false;
};
