#pragma once

/// All flags are stored in CAP namespace.
using CategoriesAndPurity = unsigned char;

/// Bit flags for CategoriesAndPurity type. Can be combined via bit operations.
namespace CAP
{
	constexpr CategoriesAndPurity none				= 0b00000000U;
	constexpr CategoriesAndPurity categoryGeneral	= 0b00000001U;
	constexpr CategoriesAndPurity categoryAnime		= 0b00000010U;
	constexpr CategoriesAndPurity categoryPeople	= 0b00000100U;
	constexpr CategoriesAndPurity puritySFW			= 0b00001000U;
	constexpr CategoriesAndPurity puritySketchy		= 0b00010000U;
	constexpr CategoriesAndPurity purityNSFW		= 0b00100000U;
}
