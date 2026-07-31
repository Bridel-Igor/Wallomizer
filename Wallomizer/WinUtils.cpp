#include "WinUtils.h"

#include <shlobj.h>
#include <Windows.h>
#include <exception>
#include <winver.h>

#pragma comment(lib, "Version.lib")

WinUtils::WinUtils()
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	PWSTR tmp_path;
	HRESULT res = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &tmp_path);
	if (res != S_OK)
	{
		CoTaskMemFree(tmp_path);
		throw std::exception("Can't get .../AppData/Roaming/ path.");
	}
	wcscpy_s(roamingNative, MAX_PATH, tmp_path);
	CoTaskMemFree(tmp_path);
	wcscat_s(roamingNative, MAX_PATH, L"\\Wallomizer\\\0");
	CreateDirectoryW(roamingNative, NULL);

	wcscpy_s(roaming, MAX_PATH, roamingNative);
	for (int j = 0; roaming[j]; j++)
		if (roaming[j] == '\\')
			roaming[j] = '/';
}

void WinUtils::updateDesktopBackground(bool isImageVisible) const
{
	wchar_t wsCurrentPathNative[MAX_PATH] = { 0 };
	if (isImageVisible)
	{
		wcscpy_s(wsCurrentPathNative, MAX_PATH, getRoamingDirNative());
		wcscat_s(wsCurrentPathNative, MAX_PATH, L"Current wallpaper.jpg");
	}
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, wsCurrentPathNative, SPIF_UPDATEINIFILE);
}

void WinUtils::flipWallpaperStyle() const
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

void WinUtils::setBackgroundColor(COLORREF color) const
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

COLORREF WinUtils::getBackgroundColor() const
{
	return GetSysColor(COLOR_BACKGROUND);
}

bool WinUtils::getAppVersion(char* version) const
{
	char* szFilename;
	if (_get_pgmptr(&szFilename) != 0)
		return false;

	// allocate a block of memory for the version info
	DWORD dummy;
	DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
	if (dwSize == 0)
		return false;
	char* data = new char[dwSize];

	// load the version info
	if (!GetFileVersionInfo(szFilename, NULL, dwSize, &data[0]))
	{
		delete[] data;
		return false;
	}

	// get version string
	LPVOID pvProductVersion = NULL;
	unsigned int iProductVersionLen = 0;
	if (!VerQueryValue(&data[0], "\\StringFileInfo\\000904b0\\ProductVersion", &pvProductVersion, &iProductVersionLen))
	{
		delete[] data;
		return false;
	}

	strcpy_s(version, iProductVersionLen, (char*)pvProductVersion);

#ifdef _DEBUG
	strcat_s(version, 16, " debug");
#endif

	delete[] data;
	return true;
}