#include "WinUtils.h"

#include <shlobj.h>
#include <Windows.h>
#include <exception>
#include <vector>
#include <wrl/client.h>

#pragma comment(lib, "Version.lib")

namespace
{
	class ComInitializer
	{
	public:
		ComInitializer()
			: m_hr(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))
		{
		}

		~ComInitializer()
		{
			if (SUCCEEDED(m_hr))
				CoUninitialize();
		}

		bool isInitialized() const noexcept
		{
			return SUCCEEDED(m_hr);
		}

	private:
		HRESULT m_hr;
	};
}

WinUtils::WinUtils()
{
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	PWSTR tmp_path = nullptr;
	const HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &tmp_path);
	if (FAILED(result))
		throw std::exception("Can't get AppData/Roaming path.");

	m_roamingPath = std::filesystem::path(tmp_path) / L"Wallomizer";

	CoTaskMemFree(tmp_path);

	std::filesystem::create_directories(m_roamingPath);
}

void WinUtils::updateDesktopBackground(bool isImageVisible) const
{
	std::filesystem::path path;
	if (isImageVisible)
		path = getRoamingDir() / L"Current wallpaper.jpg";
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, const_cast<wchar_t*>(path.c_str()), SPIF_UPDATEINIFILE);
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

void WinUtils::setBackgroundColor(Color color) const
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

WinUtils::Color WinUtils::getBackgroundColor() const
{
	return GetSysColor(COLOR_BACKGROUND);
}

std::string WinUtils::getAppVersion() const
{
	char* filename;
	if (_get_pgmptr(&filename) != 0)
		return "";

	// allocate a block of memory for the version info
	DWORD dummy = 0;
	DWORD size = GetFileVersionInfoSize(filename, &dummy);
	if (size == 0)
		return "";
	std::vector<char> data(size);

	// load the version info
	if (!GetFileVersionInfoA(filename, 0, size, data.data()))
		return "";

	// get version string
	LPVOID value = nullptr;
	UINT length = 0;
	if (!VerQueryValue(data.data(), "\\StringFileInfo\\000904b0\\ProductVersion", &value, &length))
		return "";

	std::string version = static_cast<char*>(value);

#ifdef _DEBUG
	version += " debug";
#endif

	return version;
}

void WinUtils::setStartup(bool enabled) const
{
	PWSTR rawPath = nullptr;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup, 0, nullptr, &rawPath);
	if (FAILED(hr))
		return;

	const std::filesystem::path startupPath = std::filesystem::path(rawPath) / L"Wallomizer.lnk";

	CoTaskMemFree(rawPath);

	if (!enabled)
	{
		std::error_code ec;
		std::filesystem::remove(startupPath, ec);
		return;
	}

	wchar_t currentBuffer[MAX_PATH];
	wchar_t directoryBuffer[MAX_PATH];

	GetModuleFileNameW(nullptr, currentBuffer, MAX_PATH);
	GetCurrentDirectoryW(MAX_PATH, directoryBuffer);

	createShortcut(currentBuffer, startupPath, directoryBuffer);
}

bool WinUtils::createShortcut(const std::filesystem::path& target, const std::filesystem::path& link, const std::filesystem::path& workingDirectory) const
{
	ComInitializer com;

	if (!com.isInitialized())
		return false;

	Microsoft::WRL::ComPtr<IShellLinkW> shellLink;

	if (FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink))))
		return false;

	if (FAILED(shellLink->SetPath(target.c_str())))
		return false;

	shellLink->SetDescription(L"");

	if (!workingDirectory.empty())
		shellLink->SetWorkingDirectory(workingDirectory.c_str());

	Microsoft::WRL::ComPtr<IPersistFile> persistFile;
	if (FAILED(shellLink.As(&persistFile)))
		return false;

	return SUCCEEDED(persistFile->Save(link.c_str(), TRUE));
}
