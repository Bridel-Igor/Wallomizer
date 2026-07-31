#pragma once

#include <Windows.h>

/// This class contains list of methods for getting or changing Windows parameters.
/// Functions are compatible with Windows 2000 Professional and newer versions of OS.
class WinUtils
{
public:
	WinUtils();
	WinUtils(const WinUtils&) = delete;
	WinUtils& operator=(const WinUtils&) = delete;
	WinUtils(WinUtils&&) = delete;
	WinUtils& operator=(WinUtils&&) = delete;

	/// Sets or removes wallpaper image.
	///
	/// @param isImageVisible - if true then method will update image to current one, if false will remove it.
	void updateDesktopBackground(bool isImageVisible) const;

	/// Flips wallpaper style between "fit" and "fill" positions.
	void flipWallpaperStyle() const;

	/// Sets desktop background color that will be visible with "fit" wallpaper style
	/// or when player is stopped.
	/// 
	/// @param color - color that will be set. Use macro RBG()
	void setBackgroundColor(COLORREF color) const;

	/// Gets current desktop background color.
	///
	/// @return Curent color of COLORREF type. Use GetRValue, GetGValue, GetBValue macro if needed.
	COLORREF getBackgroundColor() const;

	/// Get const pointer to string that contains path to <"user name"/AppData/Roaming/Wallomizer/> directory.
	/// Length is MAX_PATH (Windows constant).
	/// Path contains '/' separators and ends with one.
	/// If you need path with '\\' separators use getRoamingDirNative function.
	const wchar_t* getRoamingDir() const
	{
		return roaming;
	}

	/// Get const pointer to string that contains path to <"user name"\\AppData\\Roaming\\Wallomizer\\> directory.
	/// Length is MAX_PATH (Windows constant).
	/// Path contains '\\' separators and ends with one.
	/// If you need path with '/' separators use getRoamingDir function.
	const wchar_t* getRoamingDirNative() const
	{
		return roamingNative;
	}

	/// Get string that contains application version. You should provie char array to write to it.
	/// 
	/// @param version - pointer to char array of at least 16 size that will be filled.
	/// 
	/// @return true if success, false otherwise.
	bool getAppVersion(char* version) const;

private:
	wchar_t roaming[MAX_PATH];
	wchar_t roamingNative[MAX_PATH];
};