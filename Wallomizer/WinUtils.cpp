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

void WinUtils::setBackgroundColor(COLORREF color)
{
	int colors[1] = { COLOR_BACKGROUND };
	SetSysColors(1, colors, &color);

	HKEY hUserKey, hKey;
	LRESULT lResult = RegOpenCurrentUser(KEY_WRITE, &hUserKey);
	if (lResult != ERROR_SUCCESS)
		hUserKey = HKEY_CURRENT_USER;
	LSTATUS result = RegOpenKeyExA(hUserKey, "Control Panel\\Colors", 0, KEY_WRITE, &hKey);
	if (result == ERROR_SUCCESS)
	{
		char szColor[32], tmp[4];

		_itoa_s(GetRValue(color), tmp, 10);
		tmp[3] = 0;
		strcpy_s(szColor, tmp);
		strcat_s(szColor, " ");

		_itoa_s(GetGValue(color), tmp, 10);
		tmp[3] = 0;
		strcat_s(szColor, tmp);
		strcat_s(szColor, " ");

		_itoa_s(GetBValue(color), tmp, 10);
		tmp[3] = 0;
		strcat_s(szColor, tmp);

		RegSetValueExA(hKey, "Background", 0, REG_SZ, (LPBYTE)szColor, sizeof(szColor));
		RegCloseKey(hKey);
	}
}

COLORREF WinUtils::getBackgroundColor()
{
	return GetSysColor(COLOR_BACKGROUND);
}