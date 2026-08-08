#pragma once

#include <cstddef>

namespace Collection
{
	enum class Type : unsigned char
	{
		none,
		local,
		user,
		search
	};

	constexpr std::size_t getMaxPathSize(Type type) noexcept
	{
		switch (type)
		{
		case Type::none:
			return 0;
		case Type::local:
			return 260; // MAX_PATH
		case Type::user:
			return 255;
		case Type::search:
			return 1024;
		}
		return 0;
	}
};
