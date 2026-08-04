#pragma once

#include <string>

#include "CollectionTypes.h"

class WinUtils;

/// Stores wallpaper information: source collection type and path/URL.
/// Provides methods to load and open the wallpaper externally.
class Wallpaper
{
public:
	explicit Wallpaper(Collection::Type type, const wchar_t* path) :
		m_type(type),
		m_path(path)
	{}

	Wallpaper(const Wallpaper&) = delete;
	Wallpaper& operator=(const Wallpaper&) = delete;

	Wallpaper(Wallpaper&&) noexcept = default;
	Wallpaper& operator=(Wallpaper&&) = delete;

	Collection::Type getType() const noexcept { return m_type; }
	const std::wstring& getPath() const noexcept { return m_path; }

	bool loadWallpaper(const WinUtils& winUtils) const;
	void openExternally() const;

private:
	Collection::Type m_type = Collection::Type::none;
	std::wstring m_path;
};
