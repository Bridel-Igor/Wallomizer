#pragma once

#include <cstdint>
#include <atomic>

class AppState
{
private:
	enum class State : std::uint8_t
	{
		starting,
		running,
		exiting
	};

public:
	AppState() = default;
	AppState(const AppState&) = delete;
	AppState& operator=(const AppState&) = delete;
	AppState(AppState&&) = delete;
	AppState& operator=(AppState&&) = delete;

	void setFirstLaunch() noexcept		{ m_firstLaunch = true; }
	bool isFirstLaunch() const noexcept	{ return m_firstLaunch; }

	void setRunning() noexcept			{ m_state.store(State::running, std::memory_order_relaxed); }
	void setExiting() noexcept			{ m_state.store(State::exiting, std::memory_order_relaxed); }
	bool isStarting() const noexcept	{ return m_state.load(std::memory_order_relaxed) == State::starting; }
	bool isRunning() const noexcept		{ return m_state.load(std::memory_order_relaxed) == State::running; }
	bool isExiting() const noexcept		{ return m_state.load(std::memory_order_relaxed) == State::exiting; }

	void setLoading(bool value)	noexcept		{ m_loading.store(value, std::memory_order_relaxed); }
	bool isLoading() const noexcept				{ return m_loading.load(std::memory_order_relaxed); }

	void setNoWallpapers(bool value) noexcept	{ m_noWallpapers.store(value, std::memory_order_relaxed); }
	bool isNoWallpapers() const noexcept		{ return m_noWallpapers.load(std::memory_order_relaxed); }

	void setNoInternet(bool value) noexcept		{ m_noInternet.store(value, std::memory_order_relaxed); }
	bool isNoInternet() const noexcept			{ return m_noInternet.load(std::memory_order_relaxed); }

	void setFileError(bool value) noexcept		{ m_filesystemError.store(value, std::memory_order_relaxed); }
	bool isFileError() const noexcept			{ return m_filesystemError.load(std::memory_order_relaxed); }

private:
	// main thread only
	bool m_firstLaunch = false;

	// accessed by many threads
	std::atomic<State> m_state = State::starting;
	std::atomic_bool m_loading = false;
	std::atomic_bool m_noWallpapers = false;
	std::atomic_bool m_noInternet = false;
	std::atomic_bool m_filesystemError = false;
};
