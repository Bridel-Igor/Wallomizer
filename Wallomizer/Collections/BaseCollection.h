#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "CollectionTypes.h"
#include "CategoriesAndPurity.h"

class Wallpaper;
class BinaryWriter;
class BinaryReader;

class BaseCollection
{
public:
	explicit BaseCollection(CollectionType type) noexcept :
		m_type(type)
	{}
	BaseCollection(const BaseCollection&) = delete;
	BaseCollection& operator=(const BaseCollection&) = delete;
	BaseCollection(BaseCollection&&) = delete;
	BaseCollection& operator=(BaseCollection&&) = delete;
	virtual ~BaseCollection() = default;

	virtual bool saveSettings(BinaryWriter& file) const = 0;
	virtual bool loadSettings(BinaryReader& file, std::uint16_t fileVersion) = 0;
	virtual std::wstring getCollectionName() const = 0;
	virtual CategoriesAndPurity getCAP() const = 0;
	virtual Wallpaper getWallpaper(std::size_t index) const = 0;
	virtual void update() = 0;
	virtual bool isValid() const = 0;

	CollectionType getType() const noexcept		{ return m_type; }
	std::size_t getWallpaperCount() const noexcept	{ return m_wallpaperCount; }
	bool isEnabled() const noexcept					{ return m_isEnabled; }
	void enableCollection(bool isEnabled) noexcept	{ m_isEnabled = isEnabled; }

protected:
	const CollectionType m_type = CollectionType::none;
	std::uint8_t m_isEnabled = true;
	std::size_t m_wallpaperCount = 0;
};
