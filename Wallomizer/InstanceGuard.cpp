#include "InstanceGuard.h"

#include <cassert>
#include <stdexcept>
#include <Windows.h>

InstanceGuard::InstanceGuard(const char* appName)
{
	assert(appName != nullptr);

	m_hMutex = CreateMutexA(nullptr, TRUE, appName);

	if (m_hMutex == nullptr)
		throw std::runtime_error("Failed to create application mutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hMutex);
		m_hMutex = nullptr;
		throw std::runtime_error("Application is already running");
	}
}

InstanceGuard::~InstanceGuard()
{
	if (m_hMutex)
		CloseHandle(m_hMutex);
}
