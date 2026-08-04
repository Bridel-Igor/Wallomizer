#pragma once

#include <cstddef>
#include <windows.h>

namespace Collection
{
	enum class Type : unsigned char
	{
		none,
		local,
		user,
		search
	};

	constexpr size_t getMaxPathSize(Type type) noexcept
	{
		switch (type)
		{
		case Type::none:
			return 0;
		case Type::local:
			return MAX_PATH;
		case Type::user:
			return 255;
		case Type::search:
			return 1024;
		}
		return 0;
	}
};
