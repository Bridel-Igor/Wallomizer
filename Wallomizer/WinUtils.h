#pragma once

#include <string>
#include <filesystem>

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

	using Color = unsigned long;

	/// Sets or removes wallpaper image.
	void updateDesktopBackground(bool isImageVisible) const;

	/// Flips wallpaper style between "fit" and "fill" positions.
	void flipWallpaperStyle() const;

	/// Sets desktop background color that will be visible with "fit" wallpaper style
	/// or when player is stopped. You can pass COLORREF parameter.
	void setBackgroundColor(Color color) const;

	/// Gets current desktop background color. Use is as COLORREF.
	Color getBackgroundColor() const;

	/// Get path to <"user name"/AppData/Roaming/Wallomizer/> directory.
	const std::filesystem::path& getRoamingDir() const noexcept { return m_roamingPath; }

	/// Get string that contains application version.
	std::string getAppVersion() const;

	/// Add or remove app from startup.
	void setStartup(bool enabled) const;

private:
	bool createShortcut(const std::filesystem::path& target, const std::filesystem::path& link, const std::filesystem::path& workingDirectory) const;

	std::filesystem::path m_roamingPath;
};
