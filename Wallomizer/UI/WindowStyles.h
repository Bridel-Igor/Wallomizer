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

	// edit style

	extern COLORREF editFontColor;
	extern COLORREF editBkColor;
	extern COLORREF editBkInvalidColor;

	// collection items frame style

	extern COLORREF collItemFontColor;
	extern COLORREF collItemBkColor;
	extern HBRUSH collItemBkBrush;
	extern HBRUSH collFrameBkBrush;
	extern HICON hIOptions, hIOptionsHover, hIDelete, hIDeleteHover;

	// player icons

	extern HICON	hIPlay, hIPlayHover, hIPlayActive, 
					hIPause, hIPauseHover, hIPauseActive, 
					hIPrev, hIPrevHover, hIPrevDisabled, 
					hINext, hINextHover, 
					hIOpenExternal, hIOpenExternalHover;

	// check box

	extern HICON hICheckBox, hICheckBoxChecked, hICheckBoxHover, hICheckBoxCheckedHover;
}