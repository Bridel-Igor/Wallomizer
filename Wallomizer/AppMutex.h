#pragma once

#include <Windows.h>

/// Class provides functionality to limit opening of applications to only one instance.
/// If application will be opened on top of already running then exception will be thrown.
/// To use it just create object of this class right after entry point.
/// Works on Windows OS only.
class AppMutex
{
public:
	/// Constructor of application mutex.
	/// @param appName - Name of the application. Used as mutex value. 
	AppMutex(const char* appName);
	~AppMutex();

	/// Deleted as using of copy constructors will throw exception. 
	AppMutex(const AppMutex&) = delete;
	AppMutex& operator=(const AppMutex&) = delete;
	
private:
	/// Handle to appliation mutex.
	HANDLE m_hMutex = nullptr;
};