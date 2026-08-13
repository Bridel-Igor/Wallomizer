#pragma once

#include <Windows.h>

namespace UIColor
{
	// Colors
	inline constexpr COLORREF black					= RGB(0, 0, 0);
	inline constexpr COLORREF lightGray				= RGB(200, 200, 200);
	inline constexpr COLORREF white					= RGB(255, 255, 255);

	// Window
	inline constexpr COLORREF windowBk				= RGB(26, 26, 26);

	// Static controls
	inline constexpr COLORREF staticText			= RGB(129, 193, 193);
	inline constexpr COLORREF staticTitle			= RGB(220, 220, 220);

	// Edit controls
	inline constexpr COLORREF editBk				= lightGray;
	inline constexpr COLORREF editInvalidBk			= RGB(200, 100, 100);
	inline constexpr COLORREF editText				= black;

	// Push button
	inline constexpr COLORREF pbTextActive			= white;
	inline constexpr COLORREF pbTextInactive		= lightGray;
	inline constexpr COLORREF pbTopActive			= RGB(130, 130, 130);
	inline constexpr COLORREF pbBottomActive		= RGB(80, 80, 80);

	// Collection item
	inline constexpr COLORREF collectionPanelBk				= RGB(15, 15, 15);
	inline constexpr COLORREF collectionItemBk				= RGB(36, 36, 36);
	inline constexpr COLORREF collectionItemText			= lightGray;
	inline constexpr COLORREF collectionItemTextInactive	= RGB(80, 80, 80);
}
