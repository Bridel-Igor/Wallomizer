#pragma once

#include <Windows.h>

class AppMutex
{
public:
	AppMutex(const char* appName)
	{
		HANDLE hMutex = CreateMutexA(NULL, TRUE, appName);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBoxA(nullptr, "Application is already launched!", appName, MB_OK | MB_ICONEXCLAMATION);
			exit(0);
		}
	}
	~AppMutex()
	{
		if (hMutex)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
		}
	}
	AppMutex(const AppMutex&) = delete;
	AppMutex& operator=(const AppMutex&) = delete;
	
private:
	HANDLE hMutex = nullptr;
};