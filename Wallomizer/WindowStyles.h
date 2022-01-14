#pragma once

#include <Windows.h>

namespace WindowStyles
{
	// main style
	extern HFONT mainFont;
	extern HFONT titleFont;
	extern COLORREF mainFontColor;
	extern COLORREF titleFontColor;
	extern HBRUSH mainBkBrush;

	// edit style
	extern COLORREF editFontColor;
	extern COLORREF editBkColor;
	extern COLORREF editBkInvalidColor;

	// collection items frame style
	extern COLORREF collItemFontColor;
	extern COLORREF collItemBkColor;
	extern HBRUSH collItemBkBrush;
	extern HBRUSH collFrameBkBrush;

	// player icons
	extern HICON hIPlay, hIPlayActive, hIPause, hIPauseActive, hIPrevEnabled, hIPrevDisabled, hINextEnabled, hIOpenExternal;

	void initialize();
	void clear();
}