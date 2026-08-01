#pragma once

/// Enumeration of possible types of wallpaper collections.
enum class CollectionType : unsigned char
{
	none,
	local,
	user,
	search
};

/// Class that contains wallpaper info.
/// Used for containing information about current and previous wallpapers.
/// Wallpaper object used for retrieving wallpaper from specified path or URL 
/// by passing it to static loadWallpaper method of corresponding to CollectionType collection class.
class Wallpaper
{
public:
	/// Wallpaper construction.
	/// 
	/// @param type - one of values of CollectionType enum that corresponds to collection class for constructing wallpaper.
	explicit Wallpaper(CollectionType type);
	~Wallpaper();

	/// Get the collection type this wallpaper correspondes to.
	/// 
	/// @return One of the values of CollectionType enum that corresponds to collection class.
	CollectionType getType() const;

	/// Get path or URL for current wallpaper.
	/// 
	/// @return Pointer to wide char array, that contains path or URL.
	wchar_t* getPathW() const;

private:
	wchar_t* m_path = nullptr;
	CollectionType m_type = CollectionType::none;
};