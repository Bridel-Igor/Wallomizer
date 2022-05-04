#include "WindowStyles.h"
#include "resource.h"

// main style
HFONT WindowStyles::mainFont;
HFONT WindowStyles::titleFont;
HBRUSH WindowStyles::mainBkBrush;
COLORREF WindowStyles::mainFontColor;
COLORREF WindowStyles::titleFontColor;

void WindowStyles::initialize()
{
	// main style
	mainFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	titleFontColor = RGB(220, 220, 220);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));
}

void WindowStyles::clear()
{
	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);
}