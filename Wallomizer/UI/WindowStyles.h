#pragma once

#include <Windows.h>

namespace WindowStyles
{	
	void initialize();
	void clear();

	// main style
	extern HFONT mainFont;
	extern HFONT titleFont;
	extern COLORREF mainFontColor;
	extern COLORREF titleFontColor;
	extern HBRUSH mainBkBrush;

	// collection items frame style
	extern COLORREF collItemFontColor;
	extern COLORREF collItemBkColor;
	extern HBRUSH collItemBkBrush;
	extern HBRUSH collFrameBkBrush;
	extern HICON hIOptions, hIOptionsHover, hIDelete, hIDeleteHover;
}