#pragma once

/// All flags are stored in CAP namespace.
typedef unsigned char CategoriesAndPurity;

/// Bit flags for CategoriesAndPurity type. Can be combined via bit operations.
namespace CAP
{
	constexpr unsigned char none			= 0b00000000U;
	constexpr unsigned char categoryGeneral	= 0b00000001U;
	constexpr unsigned char categoryAnime	= 0b00000010U;
	constexpr unsigned char categoryPeople	= 0b00000100U;
	constexpr unsigned char puritySFW		= 0b00001000U;
	constexpr unsigned char puritySketchy	= 0b00010000U;
	constexpr unsigned char purityNSFW		= 0b00100000U;
}