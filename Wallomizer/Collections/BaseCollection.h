#pragma once

#include <cstdint>
#include <string>

#include "CollectionTypes.h"
#include "CategoriesAndPurity.h"

struct HWND__;
using HWND = HWND__*;

class Wallpaper;
class BinaryWriter;
class BinaryReader;

class BaseCollection
{
public:
	BaseCollection() = default;
	BaseCollection(const BaseCollection&) = delete;
	BaseCollection(BaseCollection&&) = delete;
	BaseCollection& operator=(const BaseCollection&) = delete;
	BaseCollection& operator=(BaseCollection&&) = delete;

	virtual ~BaseCollection() {}
	virtual bool saveSettings(BinaryWriter& file) const = 0;
	virtual bool loadSettings(BinaryReader& file, std::uint16_t fileVersion) = 0;
	virtual std::wstring getCollectionName() const = 0;
	virtual CategoriesAndPurity getCAP() const = 0;
	virtual Wallpaper getWallpaper(std::size_t index) const = 0;
	virtual void openCollectionSettingsWindow(HWND hWnd) = 0; // TODO: extract window call to UI classes or Collection Manager
	virtual void update() = 0;

	std::uint32_t getNumber() const noexcept		{ return m_number; }
	bool isEnabled() const noexcept				{ return m_isEnabled; }
	void setEnabled(bool isEnabled) noexcept	{ m_isEnabled = isEnabled; }

protected:
	Collection::Type m_type = Collection::Type::none;
	std::uint8_t m_isEnabled = true;
	bool m_isValid = false;
	std::uint32_t m_number = 0;
};
