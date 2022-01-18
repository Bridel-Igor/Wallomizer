#include "WindowStyles.h"
#include "resource.h"

// main style
HFONT WindowStyles::mainFont;
HFONT WindowStyles::titleFont;
HBRUSH WindowStyles::mainBkBrush;
COLORREF WindowStyles::mainFontColor;
COLORREF WindowStyles::titleFontColor;

// edit style
COLORREF WindowStyles::editFontColor;
COLORREF WindowStyles::editBkColor;
COLORREF WindowStyles::editBkInvalidColor;

// collection items frame style
COLORREF WindowStyles::collItemFontColor;
COLORREF WindowStyles::collItemBkColor;
HBRUSH WindowStyles::collItemBkBrush;
HBRUSH WindowStyles::collFrameBkBrush;
HICON WindowStyles::hIOptions, WindowStyles::hIDelete;

// player icons
HICON WindowStyles::hIPlay, WindowStyles::hIPlayActive, WindowStyles::hIPause, 
		WindowStyles::hIPauseActive, WindowStyles::hIPrevEnabled, WindowStyles::hIPrevDisabled, 
		WindowStyles::hINextEnabled, WindowStyles::hIOpenExternal;

// check box
HICON WindowStyles::hICheckBox, WindowStyles::hICheckBoxChecked, 
		WindowStyles::hICheckBoxHover, WindowStyles::hICheckBoxCheckedHover;

void WindowStyles::initialize()
{
	mainFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	titleFontColor = RGB(220, 220, 220);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));

	editFontColor = RGB(0, 0, 0);
	editBkColor = RGB(200, 200, 200);
	editBkInvalidColor = RGB(200, 50, 50);

	collItemFontColor = RGB(200, 200, 200);
	collItemBkColor = RGB(36, 36, 36);
	collItemBkBrush = CreateSolidBrush(collItemBkColor);
	collFrameBkBrush = CreateSolidBrush(RGB(15, 15, 15));
	hIDelete =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_DELETE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptions =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_OPTIONS),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

	hIPlay =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PLAY),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPlayActive =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PLAY_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPause =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PAUSE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPauseActive =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PAUSE_ACTIVE),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevEnabled =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PREV),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevDisabled =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_PREV_DISABLED),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hINextEnabled =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_NEXT),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOpenExternal =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_OPEN_EXTERNAL),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

	hICheckBox =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_CHECKBOX),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxChecked =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_CHECKBOX_CHECKED),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_CHECKBOX_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxCheckedHover =	(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_I_CHECKBOX_CHECKED_HOVER), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

void WindowStyles::clear()
{
	DestroyIcon(hICheckBoxCheckedHover);
	DestroyIcon(hICheckBoxHover);
	DestroyIcon(hICheckBoxChecked);
	DestroyIcon(hICheckBox);

	DestroyIcon(hIPlay);
	DestroyIcon(hIPlayActive);
	DestroyIcon(hIPause);
	DestroyIcon(hIPauseActive);
	DestroyIcon(hIPrevEnabled);
	DestroyIcon(hIPrevDisabled);
	DestroyIcon(hINextEnabled);
	DestroyIcon(hIOpenExternal);
	DestroyIcon(hIDelete);
	DestroyIcon(hIOptions);

	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);

	DeleteObject(collItemBkBrush);
	DeleteObject(collFrameBkBrush);
}