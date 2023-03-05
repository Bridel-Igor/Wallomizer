#include <shlobj.h>
#include <Windows.h>
#include <exception>
#include <winver.h>
#pragma comment(lib, "Version.lib")

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

bool Filesystem::getAppVersion(char* version)
{
	char* szFilename;
	if (_get_pgmptr(&szFilename) != 0)
		return false;

    // allocate a block of memory for the version info
    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
    if (dwSize == 0)
        return false;
	char *data = new char[dwSize];

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
	delete[] data;
	return true;
}