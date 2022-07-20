#include <shlobj.h>
#include <Windows.h>
#include <exception>

#include "Filesystem.h"

namespace Filesystem
{
	wchar_t roaming[MAX_PATH];
	wchar_t roamingNative[MAX_PATH];
}

void Filesystem::initialize()
{
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

void Filesystem::getRoamingDir(wchar_t* path)
{
	wcscpy_s(path, MAX_PATH, roaming);
}

void Filesystem::getRoamingDirNative(wchar_t* path)
{
	wcscpy_s(path, MAX_PATH, roamingNative);
}