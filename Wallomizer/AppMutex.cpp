#include <exception>

#include "AppMutex.h"

AppMutex::AppMutex(const char* appName)
{
	m_hMutex = CreateMutexA(NULL, TRUE, appName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		throw std::exception("Application is already running!");
}

AppMutex::~AppMutex()
{
	if (m_hMutex)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
	}
}