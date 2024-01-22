#pragma once

#include <Windows.h>

/// This namespace contains list of functions that change parameters or registry of Windows.
/// Functions are compatible with Windows 2000 Professional and newer versions of OS.
namespace WinUtils
{
	/// Sets image as background wallpaper or removes background wallpaper based on Delay::slideshowStatus.
	void updateDesktopBackground();

	/// Flips wallpaper style between "fit" and "fill" positions.
	void flipWallpaperStyle();

	/// Sets desktop background color that will be visible with "fit" wallpaper style
	/// or when player is stopped.
	/// 
	/// @param color - color that will be set. Use macro RBG()
	void setBackgroundColor(COLORREF color);

	/// Gets current desktop background color.
	///
	/// @return Curent color of COLORREF type. Use GetRValue, GetGValue, GetBValue macro if needed.
	COLORREF getBackgroundColor();
}