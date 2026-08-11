#pragma once

#include <cstdint>
#include <bitset>

/// RAII wrapper for a unique 16-bit UI component identifier.
/// Automatically reserves an identifier on construction and releases it on destruction.
class HMenuId
{
public:
	/// Reserves a unique identifier.
	/// @throws std::runtime_error if all available identifiers are already in use.
	HMenuId();
	HMenuId(const HMenuId&) = delete;
	HMenuId& operator=(const HMenuId&) = delete;
	HMenuId(HMenuId&&) = delete;
	HMenuId& operator=(HMenuId&&) = delete;
	~HMenuId();

	/// @return Unique identifier assigned to this object.
	std::uint16_t value() const noexcept { return m_value; }

private:
	/// Tracks which identifiers in the [1000, 1999] range are currently in use.
	static std::bitset<1000> s_usedValues;

	std::uint16_t m_value = 0;
};
