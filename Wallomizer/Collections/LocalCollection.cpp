#include "LocalCollection.h"

#include <cwchar>

#include "CollectionManager.h"
#include "WinUtils.h"
#include "Wallpaper.h"
#include "BinaryIO.h"

namespace
{
	bool isImage(const std::filesystem::path& path)
	{
		static constexpr wchar_t extensions[][7] =
		{ L".jpg", L".jpeg", L".bmp", L".dib", L".png", L".jfif", L".jpe", L".gif", L".tif", L".tiff",
		L".wdp", L".heic", L".heif", L".heics", L".heifs", L".avci", L".avcs", L".avif", L".avifs", L".webp" };

		auto extension = path.extension().wstring();

		for (const auto& supported : extensions)
			if (_wcsicmp(extension.c_str(), supported) == 0)
				return true;
		return false;
	}
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
	getWallpaper();
}

Wallpaper LocalCollection::getWallpaper(std::size_t index)
{
	Wallpaper wallpaper = Wallpaper::getEmptyWallpaper();
	m_wallpaperCount = 0;
	if (m_path.empty() || !m_isEnabled)
		return wallpaper;

	std::error_code ec;
	if (m_isRecursive)
	{
		for (auto it = std::filesystem::recursive_directory_iterator(m_path, std::filesystem::directory_options::skip_permission_denied, ec);
			it != std::filesystem::recursive_directory_iterator();
			it.increment(ec))
		{
			if (ec)
			{
				ec.clear();
				continue;
			}

			const auto& entry = *it;

			if (entry.is_directory(ec))
			{
				if (ec)
				{
					ec.clear();
					continue;
				}

				if (m_winUtils.isHiddenOrSystem(entry.path()))
				{
					it.disable_recursion_pending();
					continue;
				}
			}

			if (isImage(entry))
			{
				if (m_wallpaperCount == index)
					wallpaper = Wallpaper(m_type, entry.path().generic_wstring());
				m_wallpaperCount++;
			}
		}
	}
	else
	{
		for (auto& path : std::filesystem::directory_iterator(m_path, std::filesystem::directory_options::skip_permission_denied, ec))
		{
			if (ec)
			{
				ec.clear();
				continue;
			}
			if (isImage(path))
			{
				if (m_wallpaperCount == index)
					wallpaper = Wallpaper(m_type, path.path().generic_wstring());
				m_wallpaperCount++;
			}
		}
	}

	m_collectionManager.recountWallpapers();
	return wallpaper;
}

bool LocalCollection::loadWallpaper(const std::filesystem::path& sourcePath, const WinUtils& winUtils)
{
	std::error_code ec;
	std::filesystem::path loadedPath = winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	std::filesystem::copy_file(sourcePath, loadedPath, std::filesystem::copy_options::overwrite_existing, ec);
	
	if (ec)
	{
		std::error_code ignore;
		std::filesystem::remove(loadedPath, ignore);
		return false;
	}

	return true;
}

void LocalCollection::openWallpaperExternal(const std::filesystem::path& sourcePath, const WinUtils& winUtils)
{
	winUtils.openInFolderAsync(sourcePath);
}

bool LocalCollection::isValid() const
{
	return m_type == CollectionType::local 
		&& !m_path.empty();
}
