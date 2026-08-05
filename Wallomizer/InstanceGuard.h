#pragma once

/// RAII guard that ensures only one instance of an application is running.
/// Throws an exception if another instance already owns the same mutex.
/// Create an object immediately after the application entry point.
/// Windows-specific implementation.
class InstanceGuard
{
public:
	/// Creates a named application instance mutex.
	/// @param appName - Unique name used to identify the application instance. 
	explicit InstanceGuard(const char* appName);
	~InstanceGuard();

	/// Copy and move operations are disabled because the object owns a unique system resource. 
	InstanceGuard(const InstanceGuard&) = delete;
	InstanceGuard& operator=(const InstanceGuard&) = delete;
	InstanceGuard(InstanceGuard&&) = delete;
	InstanceGuard& operator=(InstanceGuard&&) = delete;
	
private:
	/// Opaque handle to the operating system mutex object.
	void* m_hMutex = nullptr;
};
