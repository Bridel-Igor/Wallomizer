#include "WindowStyles.h"
#include "resource.h"

// main style
HFONT WindowStyles::mainFont;
HFONT WindowStyles::titleFont;
HBRUSH WindowStyles::mainBkBrush;
COLORREF WindowStyles::mainFontColor;
COLORREF WindowStyles::titleFontColor;

// collection items frame style
COLORREF WindowStyles::collItemFontColor;
COLORREF WindowStyles::collItemBkColor;
HBRUSH WindowStyles::collItemBkBrush;
HBRUSH WindowStyles::collFrameBkBrush;
HICON	WindowStyles::hIOptions, WindowStyles::hIOptionsHover, 
		WindowStyles::hIDelete, WindowStyles::hIDeleteHover;

void WindowStyles::initialize()
{
	// main style
	mainFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	titleFontColor = RGB(220, 220, 220);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));

	// collection items frame style
	collItemFontColor = RGB(200, 200, 200);
	collItemBkColor = RGB(36, 36, 36);
	collItemBkBrush = CreateSolidBrush(collItemBkColor);
	collFrameBkBrush = CreateSolidBrush(RGB(15, 15, 15));
	hIDelete =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIDeleteHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptions =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptionsHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

void WindowStyles::clear()
{
	DeleteObject(collItemBkBrush);
	DeleteObject(collFrameBkBrush);
	DestroyIcon(hIDelete);
	DestroyIcon(hIDeleteHover);
	DestroyIcon(hIOptions);
	DestroyIcon(hIOptionsHover);

	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);
}