#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <experimental/filesystem>
#include <string>
#include <shlobj_core.h>

#include "LocalCollection.h"
#include "SetLocalCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

bool isImage(std::experimental::filesystem::path path)
{
	const char* extensions[] = { ".jpg", ".jpeg", ".bmp", ".dib", ".png", ".jfif", ".jpe", ".gif", ".tif", ".tiff",
								".wdp", ".heic", ".heif", ".heics", ".heifs", ".avci", ".avcs", ".avif", ".avifs" };
	for (auto extension : extensions)
		if (path.extension().compare((std::experimental::filesystem::path)extension) == 0)
			return true;
	return false;
}

bool LocalCollection::saveSettings(FILE* pFile) const
{
	if (pFile == NULL)
		return false;
	const CollectionType collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(LocalCollection::LocalCollectionSettings), 1, pFile);
	return true;
}

bool LocalCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	
	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fread(&settings, sizeof(LocalCollection::LocalCollectionSettings), 1, pFile);

	std::experimental::filesystem::path dirPath{ settings.wsDirectoryPath };
	m_uiNumber = 0;
	if (m_isEnabled)
	{
		if (settings.bRecursive)
		{
			for (auto& path : std::experimental::filesystem::recursive_directory_iterator(dirPath))
				if (isImage(path))
					m_uiNumber++;
		}
		else
		{
			for (auto& path : std::experimental::filesystem::directory_iterator(dirPath))
				if (isImage(path))
					m_uiNumber++;
		}
	}
	return true;
}

void LocalCollection::getCollectionName(wchar_t* pwsName, size_t size) const
{
	wcscpy_s(pwsName, size, L" Local: ");
	wcscat_s(pwsName, 255, settings.wsDirectoryPath);
}

Wallpaper* LocalCollection::getWallpaperInfo(unsigned int index) const
{
	Wallpaper* pWallpaper = nullptr;
	if (settings.wsDirectoryPath[0] == L'\0' || m_uiNumber <= 0)
		return pWallpaper;
	unsigned int i = 0;
	std::experimental::filesystem::path dirPath{ settings.wsDirectoryPath };

	if (settings.bRecursive)
	{
		for (auto& path : std::experimental::filesystem::recursive_directory_iterator(dirPath))
			if (isImage(path))
			{
				if (i == index)
				{
					pWallpaper = new Wallpaper(CollectionType::local);
					wcscpy_s(pWallpaper->getPathW(), MAX_PATH, path.path().generic_wstring().c_str());
					pWallpaper->getPathW()[wcslen(pWallpaper->getPathW())] = '\0';
					return pWallpaper;
				}
				i++;
			}
	}
	else
	{
		for (auto& path : std::experimental::filesystem::directory_iterator(dirPath))
			if (isImage(path))
			{
				if (i == index)
				{
					pWallpaper = new Wallpaper(CollectionType::local);
					wcscpy_s(pWallpaper->getPathW(), MAX_PATH, path.path().generic_wstring().c_str());
					pWallpaper->getPathW()[wcslen(pWallpaper->getPathW())] = '\0';
					return pWallpaper;
				}
				i++;
			}
	}

	return pWallpaper;
}

void LocalCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetLocalCollectionWindow setLocalCollectionWindow(hCaller, m_pCollectionManager, this);
	setLocalCollectionWindow.windowLoop();
}

bool LocalCollection::loadWallpaper(const Wallpaper* pWallpaper)
{
	char buf[BUFSIZ];
	size_t size;
	FILE* pSource = nullptr, * pDestination = nullptr;
	_wfopen_s(&pSource, pWallpaper->getPathW(), L"rb");
	wchar_t path[MAX_PATH];
	Filesystem::getRoamingDir(path);
	wcscat_s(path, MAX_PATH, L"Loaded wallpaper.dat");
	_wfopen_s(&pDestination, path, L"wb");
	if (pSource == nullptr || pDestination == nullptr)
		return false;
	while ((size = fread(buf, 1, BUFSIZ, pSource)) != 0)
		fwrite(buf, 1, size, pDestination);
	fclose(pSource);
	fclose(pDestination);
	return true;
}

void LocalCollection::openWallpaperExternal(const Wallpaper* pWallpaper)
{
	wchar_t imgPath[MAX_PATH];
	wcscpy_s(imgPath, MAX_PATH, pWallpaper->getPathW());

	for (int j = 0; j < wcslen(imgPath); j++)
		if (imgPath[j] == '/')
			imgPath[j] = '\\';

	__unaligned ITEMIDLIST* pidl = ILCreateFromPathW(imgPath);
	if (pidl) 
	{
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
		return;
	}
}