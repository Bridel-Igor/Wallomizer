#pragma once

#include <cstdint>

enum class CollectionType : std::uint8_t
{
	none,
	local,
	user,
	search
};
