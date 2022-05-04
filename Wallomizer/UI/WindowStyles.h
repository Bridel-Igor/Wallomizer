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
}