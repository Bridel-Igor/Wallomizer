#include "LocalCollection.h"

#include <filesystem>
#include <shlobj_core.h>

#include "CollectionManager.h"
#include "WinUtils.h"
#include "Wallpaper.h"
#include "BinaryIO.h"

bool isImage(std::filesystem::path path)
{
	const char* extensions[] = { ".jpg", ".jpeg", ".bmp", ".dib", ".png", ".jfif", ".jpe", ".gif", ".tif", ".tiff",
								".wdp", ".heic", ".heif", ".heics", ".heifs", ".avci", ".avcs", ".avif", ".avifs" };
	for (auto extension : extensions)
		if (path.extension().compare((std::filesystem::path)extension) == 0)
			return true;
	return false;
}

bool LocalCollection::saveSettings(BinaryWriter& file) const
{
	return file.write(m_type)
		&& file.write(m_isEnabled)
		&& file.write(m_isRecursive)
		&& file.write(m_path);
}

bool LocalCollection::loadSettings(BinaryReader& file, std::uint16_t fileVersion)
{
	switch (fileVersion)
	{
	case 4U:
		return file.read(m_isEnabled)
			&& file.read(m_isRecursive)
			&& file.read(m_path)
			&& isValid();

	case 3U:
	{
		struct LegacyDataV3
		{
			wchar_t path[255] = L"";
			bool recursive = false;
		};
		LegacyDataV3 oldData;
		if (!file.read(m_isEnabled)
			|| !file.read(oldData))
			return false;
		m_isRecursive = oldData.recursive;
		m_path = oldData.path;
		return isValid();
	}

	default: 
		return false;
	}
}

void LocalCollection::update()
{
	m_wallpaperCount = 0;
	if (!m_isEnabled)
		return;

	if (m_isRecursive)
	{
		for (auto& path : std::filesystem::recursive_directory_iterator(m_path))
			if (isImage(path))
				m_wallpaperCount++;
	}
	else
	{
		for (auto& path : std::filesystem::directory_iterator(m_path))
			if (isImage(path))
				m_wallpaperCount++;
	}
}

std::wstring LocalCollection::getCollectionName() const
{
	std::wstring name = L" Local: ";
	name += m_path;
	return name;
}

Wallpaper LocalCollection::getWallpaper(std::size_t index) const
{
	if (m_path.empty() || m_wallpaperCount == 0)
		return Wallpaper::getEmptyWallpaper();

	std::size_t current = 0;
	if (m_isRecursive)
	{
		for (auto& path : std::filesystem::recursive_directory_iterator(m_path))
			if (isImage(path) && current++ == index)
				return Wallpaper(m_type, path.path().generic_wstring());
	}
	else
	{
		for (auto& path : std::filesystem::directory_iterator(m_path))
			if (isImage(path) && current++ == index)
				return Wallpaper(m_type, path.path().generic_wstring());
	}

	return Wallpaper::getEmptyWallpaper();
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

bool LocalCollection::isValid() const
{
	return m_type == Collection::Type::local 
		&& !m_path.empty();
}
