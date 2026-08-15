#pragma once

#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <optional>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

class IWindow;

/// Owns the lifetime of a window running in a dedicated UI thread.
///
/// The window is constructed and destroyed entirely within the worker thread.
/// start() starts the worker thread and returns immediately. Use
/// waitUntilReady() when the caller needs to ensure that the window has been
/// successfully constructed.
///
/// If start() is called while the window is already running, no new window
/// is created. The existing window is brought to the foreground instead.
///
/// Constructor arguments are stored by value before the worker thread is
/// started. Use std::ref() for objects that must be passed by reference.
///
/// requestClose() requests the window to close and waits for the worker
/// thread to finish. Exceptions thrown by the worker thread are captured and
/// rethrown by the controlling thread when the worker is joined.
template <class T>
class UIThreadedWindow
{
	static_assert(std::is_base_of_v<IWindow, T>, "UIThreadedWindow<T>: T must derive from IWindow");

public:
	template <class... Args>
	void start(Args&&... args)
	{
		if (m_thread.joinable())
		{
			if (!m_finished)
			{
				// Focusing the window, in case it's already opened.
				waitUntilReady();
				m_window->focus();
				return;
			}
			m_thread.join();
		}

		m_finished = false;
		m_exception = nullptr;

		auto arguments = std::make_tuple(std::forward<Args>(args)...);
		
		// Signals that the window has been constructed.
		std::promise<void> ready;
		m_ready = ready.get_future().share();

		m_thread = std::thread(
			[
				this, 
				arguments = std::move(arguments),
				ready = std::move(ready)
			]() mutable
			{
				try
				{
					// Construct the window.
					std::apply(
						[this](auto&&... args)
						{
							m_window.emplace(std::forward<decltype(args)>(args)...);
						},
						std::move(arguments)
					);
					// The window is constructed and is ready to process messages.
					ready.set_value();

					// Runs the whole lifetime of the window.
					m_window->windowLoop();

					// Destroying the window.
					m_window.reset();
					m_finished = true;
				}
				catch (...)
				{
					// store the exception to propagate it during join().
					m_exception = std::current_exception();

					try
					{
						// Notify waitUntilReady() in case of construction failure.
						ready.set_exception(m_exception);
					}
					catch (...)
					{
					}
				}
			}
		);
	}

	/// Waits until the window has been successfully constructed.
	/// Throws if window construction failed or when called before start().
	void waitUntilReady()
	{
		if (!m_ready.valid())
			throw std::logic_error("UIThreadedWindow::waitUntilReady() called before start()");

		try
		{
			m_ready.get();
		}
		catch (...)
		{
			join();
			throw;
		}
	}

	/// Requests the window to close and waits for its UI thread to finish.
	void requestClose()
	{
		if (!m_thread.joinable())
			return;

		/// In case the window is still under construction.
		waitUntilReady();

		if (m_window)
			m_window->requestClose();

		join();
	}

private:
	/// Reclaims the worker thread and propagates an exception produced by it.
	void join()
	{
		if (m_thread.joinable())
			m_thread.join();

		if (m_exception)
			std::rethrow_exception(m_exception);
	}

private:
	/// The window object itself. It is constructed, and destroyed by the worker thread.
	std::optional<T> m_window;

	/// Owns the thread in which the window lives.
	/// Needs to be joined even after the window has been destroyed.
	std::thread m_thread;

	/// Becomes ready after the window has been successfully constructed.
	std::shared_future<void> m_ready;

	/// Written by the worker thread and read by the controlling thread.
	/// Becomes true when the window has already been destroyed.
	std::atomic_bool m_finished = false;

	/// Exception captured from the worker thread. It is rethrown by join() on the controlling thread.
	std::exception_ptr m_exception;
};
