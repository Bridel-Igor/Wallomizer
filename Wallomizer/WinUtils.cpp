#include <Windows.h>

#include "WinUtils.h"
#include "Filesystem.h"
#include "Delay.h"

void WinUtils::updateDesktopBackground()
{
	wchar_t wsCurrentPathNative[MAX_PATH] = { 0 };
	if (Delay::slideshowStatus != Delay::SlideshowStatus::stopped)
	{
		Filesystem::getRoamingDirNative(wsCurrentPathNative);
		wcscat_s(wsCurrentPathNative, MAX_PATH, L"Current wallpaper.jpg");
	}
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, wsCurrentPathNative, SPIF_UPDATEINIFILE);
}

void WinUtils::flipWallpaperStyle()
{
	HKEY hUserKey, hKey;
	LRESULT lResult = RegOpenCurrentUser(KEY_ALL_ACCESS, &hUserKey);
	if (lResult != ERROR_SUCCESS)
		hUserKey = HKEY_CURRENT_USER;
	LSTATUS result = RegOpenKeyExA(hUserKey, "Control Panel\\Desktop", 0, KEY_ALL_ACCESS, &hKey);
	if (result == ERROR_SUCCESS)
	{
		const BYTE fit[3] = "6";
		const BYTE fill[3] = "10";
		TCHAR style[32] = "";
		DWORD size = sizeof(style);
		LSTATUS resultQuery = RegQueryValueExA(hKey, "WallpaperStyle", 0, NULL, (LPBYTE)style, &size);
		RegSetValueExA(hKey, "WallpaperStyle", 0, REG_SZ, (LPBYTE)((resultQuery != ERROR_SUCCESS) || (style[0] != fit[0])) ? fit : fill, 3);
		RegCloseKey(hKey);
	}
}
