#include "LocalCollection.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <string>
#include <shlobj_core.h>

#include "SetLocalCollectionWindow.h"
#include "CollectionManager.h"
#include "WinUtils.h"

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
	const Collection::Type collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(LocalCollection::LocalCollectionSettings), 1, pFile);
	return true;
}

bool LocalCollection::loadSettings(FILE* pFile, unsigned short fileVersion)
{
	if (pFile == NULL)
		return false;
	
	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);

	if (fileVersion >= 2U && fileVersion <= CollectionManager::COLLECTION_MANAGER_FILE_VERSION)
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

Wallpaper LocalCollection::getWallpaper(unsigned int index) const
{
	if (settings.wsDirectoryPath[0] == L'\0' || m_uiNumber <= 0)
		return Wallpaper::getEmptyWallpaper();
	unsigned int i = 0;
	std::experimental::filesystem::path dirPath{ settings.wsDirectoryPath };

	if (settings.bRecursive)
	{
		for (auto& path : std::experimental::filesystem::recursive_directory_iterator(dirPath))
			if (isImage(path))
			{
				if (i == index)
				{
					return Wallpaper(Collection::Type::local, path.path().generic_wstring());
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
					return Wallpaper(Collection::Type::local, path.path().generic_wstring());
				}
				i++;
			}
	}

	return Wallpaper::getEmptyWallpaper();
}

void LocalCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetLocalCollectionWindow setLocalCollectionWindow(hCaller, m_collectionManager, this);
	setLocalCollectionWindow.windowLoop();
}

bool LocalCollection::loadWallpaper(std::filesystem::path sourcePath, const WinUtils& winUtils)
{
	std::error_code ec;
	std::filesystem::path loadedPath = winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::copy_file(sourcePath, loadedPath, std::filesystem::copy_options::overwrite_existing, ec);
	
	if (ec)
	{
		std::error_code ignore;
		std::filesystem::remove(loadedPath);
		return false;
	}

	return true;
}

void LocalCollection::openWallpaperExternal(std::filesystem::path sourcePath)
{
	PIDLIST_ABSOLUTE pidl = ILCreateFromPathW(sourcePath.make_preferred().c_str());
	if (pidl) 
	{
		SHOpenFolderAndSelectItems(pidl, 0, nullptr, 0);
		ILFree(pidl);
	}
}
