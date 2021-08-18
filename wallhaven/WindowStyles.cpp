#include "WindowStyles.h"

// main style
HFONT WindowStyles::mainFont;
HBRUSH WindowStyles::mainBkBrush;
COLORREF WindowStyles::mainFontColor;

// edit style
COLORREF WindowStyles::editFontColor;
COLORREF WindowStyles::editBkColor;

// collection items frame style
COLORREF WindowStyles::collItemFontColor;
COLORREF WindowStyles::collItemBkColor;
HBRUSH WindowStyles::collItemBkBrush;
HBRUSH WindowStyles::collFrameBkBrush;

void WindowStyles::initialize()
{
	mainFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));

	editFontColor = RGB(0, 0, 0);
	editBkColor = RGB(200, 200, 200);

	collItemFontColor = RGB(200, 200, 200);
	collItemBkColor = RGB(36, 36, 36);
	collItemBkBrush = CreateSolidBrush(collItemBkColor);
	collFrameBkBrush = CreateSolidBrush(RGB(15, 15, 15));
}

void WindowStyles::clear()
{
	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);

	DeleteObject(collItemBkBrush);
	DeleteObject(collFrameBkBrush);
}