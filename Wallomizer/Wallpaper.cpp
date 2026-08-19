#include "Wallpaper.h"

#include "LocalCollection.h"
#include "UserCollection.h"
#include "SearchCollection.h"

bool Wallpaper::loadWallpaper(const WinUtils& winUtils) const
{
	switch (m_type)
	{
	case CollectionType::local:
		return LocalCollection::loadWallpaper(m_path, winUtils);

	case CollectionType::user:
		return UserCollection::loadWallpaper(m_path, winUtils);

	case CollectionType::search:
		return SearchCollection::loadWallpaper(m_path, winUtils);

	default:
		return false;
	}
}

void Wallpaper::openExternally(const WinUtils& winUtils) const
{
	switch (m_type)
	{
	case CollectionType::local:
		LocalCollection::openWallpaperExternal(m_path, winUtils);
		break;

	case CollectionType::user:
		UserCollection::openWallpaperExternal(m_path);
		break;

	case CollectionType::search:
		SearchCollection::openWallpaperExternal(m_path);
		break;

	default:
		break;
	}
}
