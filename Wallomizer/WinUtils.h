#pragma once

/// This namespace contains list of functions that change parameters or registry of Windows.
/// Functions are compatible with Windows 2000 Professional and newer versions of OS.
namespace WinUtils
{
	/// Sets image as background wallpaper or removes background wallpaper based on Delay::slideshowStatus.
	void updateDesktopBackground();

	/// Flips wallpaper style between "fit" and "fill" positions.
	void flipWallpaperStyle();
}