#pragma once

/// This namespace contains list of functions that provide interactions between application and filesystem.
/// Intended for using on Windows operating system only.
/// To use its functions initialize() must be called first.
namespace Filesystem
{
	/// Function initializes all variables needed for another functions of this namespace.
	/// Creates necessary directories if they aren't existing yet.
	void initialize();

	/// Get string that contains path to <"user name"/AppData/Roaming/Wallomizer/> directory. You should provide wide char array to write it to.
	/// Path contains '/' separators and ends with one. If you need path with '\\' separators use getRoamingDirNative function.
	/// 
	/// @param path - pointer to wide char array of at least MAX_PATH (Windows constant) size that will be filled.
	void getRoamingDir(wchar_t* path);

	// Following example contains "\\\\" separators insted of "\\" only for purpose of correct VS function description display.

	/// Get string that contains path to <"user name"\\\\AppData\\\\Roaming\\\\Wallomizer\\\\> directory. You should provide wide char array to write it to.
	/// Path contains '\\\\' separators and ends with one. If you need path with '/' separators use getRoamingDir function.
	/// 
	/// @param path - pointer to wide char array of at least MAX_PATH (Windows constant) size that will be filled.
	void getRoamingDirNative(wchar_t* path);

	/// Get string that contains application version. You should provie char array to write to it.
	/// 
	/// @param version - pointer to char array of at least 16 size that will be filled.
	/// 
	/// @return true if success, false otherwise.
	bool getAppVersion(char* version);

	/// Current file version of CollectionManager.dat file.
	constexpr unsigned short COLLECTION_MANAGER_FILE_VERSION = 3U;
}