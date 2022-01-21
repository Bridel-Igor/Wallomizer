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
HICON	WindowStyles::hIOptions, WindowStyles::hIOptionsHover, 
		WindowStyles::hIDelete, WindowStyles::hIDeleteHover;

// player icons
HICON	WindowStyles::hIPlay, WindowStyles::hIPlayHover, WindowStyles::hIPlayActive, 
		WindowStyles::hIPause, WindowStyles::hIPauseHover, WindowStyles::hIPauseActive, 
		WindowStyles::hIPrev, WindowStyles::hIPrevHover, WindowStyles::hIPrevDisabled,
		WindowStyles::hINext, WindowStyles::hINextHover,
		WindowStyles::hIOpenExternal, WindowStyles::hIOpenExternalHover;

// check box
HICON	WindowStyles::hICheckBox, WindowStyles::hICheckBoxChecked, 
		WindowStyles::hICheckBoxHover, WindowStyles::hICheckBoxCheckedHover;

// color button
HICON WindowStyles::hICheckWhite, WindowStyles::hICheckBlack, WindowStyles::hIColorEmpty;

void WindowStyles::initialize()
{
	// main style
	mainFont = CreateFontA(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	titleFont = CreateFontA(15, 0, 0, 0, FW_SEMIBOLD, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	mainFontColor = RGB(129, 193, 193);
	titleFontColor = RGB(220, 220, 220);
	mainBkBrush = CreateSolidBrush(RGB(26, 26, 26));

	// edit style
	editFontColor = RGB(0, 0, 0);
	editBkColor = RGB(200, 200, 200);
	editBkInvalidColor = RGB(200, 50, 50);

	// collection items frame style
	collItemFontColor = RGB(200, 200, 200);
	collItemBkColor = RGB(36, 36, 36);
	collItemBkBrush = CreateSolidBrush(collItemBkColor);
	collFrameBkBrush = CreateSolidBrush(RGB(15, 15, 15));
	hIDelete =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIDeleteHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_DELETE_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptions =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOptionsHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPTIONS_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

	// player icons
	hIPlay =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY),						IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPlayHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPlayActive =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PLAY_ACTIVE),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPause =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPauseHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPauseActive =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PAUSE_ACTIVE),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrev =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV),						IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIPrevDisabled =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_PREV_DISABLED),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hINext =					(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT),						IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hINextHover =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_NEXT_HOVER),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOpenExternal =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIOpenExternalHover =		(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_OPEN_EXTERNAL_HOVER),		IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

	// check box
	hICheckBox =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxChecked =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxCheckedHover =	(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

	// color button
	hICheckWhite =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_WHITE),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBlack =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_BLACK),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hIColorEmpty =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_COLOR_EMPTY),				IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

void WindowStyles::clear()
{
	DestroyIcon(hICheckBoxCheckedHover);
	DestroyIcon(hICheckBoxHover);
	DestroyIcon(hICheckBoxChecked);
	DestroyIcon(hICheckBox);

	DestroyIcon(hIPlay);
	DestroyIcon(hIPlayHover);
	DestroyIcon(hIPlayActive);
	DestroyIcon(hIPause);
	DestroyIcon(hIPauseHover);
	DestroyIcon(hIPauseActive);
	DestroyIcon(hIPrev);
	DestroyIcon(hIPrevHover);
	DestroyIcon(hIPrevDisabled);
	DestroyIcon(hINext); 
	DestroyIcon(hINextHover);
	DestroyIcon(hIOpenExternal);
	DestroyIcon(hIOpenExternalHover);

	DestroyIcon(hIDelete);
	DestroyIcon(hIDeleteHover);
	DestroyIcon(hIOptions);
	DestroyIcon(hIOptionsHover);

	DeleteObject(mainBkBrush);
	DeleteObject(mainFont);
	DeleteObject(titleFont);

	DeleteObject(collItemBkBrush);
	DeleteObject(collFrameBkBrush);

	DeleteObject(hICheckBlack);
	DeleteObject(hICheckWhite);
	DeleteObject(hIColorEmpty);
}