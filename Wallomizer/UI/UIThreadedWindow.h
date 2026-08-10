#pragma once

#include <thread>
#include <exception>

class App;

template <class T>
class UIThreadedWindow
{
public:
	explicit UIThreadedWindow(App& app)
		:m_thread([this, &app]
		{
			try
			{
				T window(app);
				window.windowLoop();
			}
			catch (...)
			{
				m_exception = std::current_exception();
			}
		})
	{}

	void join()
	{
		m_thread.join();
		if (m_exception)
			std::rethrow_exception(m_exception);
	}

private:
	std::thread m_thread;
	std::exception_ptr m_exception;
};
