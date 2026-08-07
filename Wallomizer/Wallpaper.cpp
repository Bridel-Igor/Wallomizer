#include "Wallpaper.h"

#include "LocalCollection.h"
#include "UserCollection.h"
#include "SearchCollection.h"

bool Wallpaper::loadWallpaper(const WinUtils& winUtils) const
{
	switch (m_type)
	{
	case Collection::Type::local:
		return LocalCollection::loadWallpaper(m_path, winUtils);

	case Collection::Type::user:
		return UserCollection::loadWallpaper(m_path, winUtils);

	case Collection::Type::search:
		return SearchCollection::loadWallpaper(m_path, winUtils);

	default:
		return false;
	}
}

void Wallpaper::openExternally() const
{
	switch (m_type)
	{
	case Collection::Type::local:
		LocalCollection::openWallpaperExternal(m_path);
		break;

	case Collection::Type::user:
		UserCollection::openWallpaperExternal(m_path);
		break;

	case Collection::Type::search:
		SearchCollection::openWallpaperExternal(m_path);
		break;

	default:
		break;
	}
}
