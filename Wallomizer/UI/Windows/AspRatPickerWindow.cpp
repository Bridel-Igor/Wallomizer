#include "AspRatPickerWindow.h"

AspRatPickerWindow::AspRatPickerWindow(HWND hCaller, char* sAspRat) :
	IWindow("Ratio", "Aspect Ratio Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 315, 195),
	m_hCaller(hCaller),
	m_sAspRat(sAspRat),
	btnAllWide		(hWnd(), "All Wide",	10,		35,		145,	20),
	btnAllPortrait	(hWnd(), "All Portrait",160,	35,		70,		20),
	stWide			(hWnd(), "Wide",		10,		10,		70,		20, SS_CENTER),
	stUltrawide		(hWnd(), "Ultrawide",	85,		10,		70,		20, SS_CENTER),
	stPortrait		(hWnd(), "Portrait",	160,	10,		70,		20, SS_CENTER),
	stSquare		(hWnd(), "Square",		235,	10,		70,		20, SS_CENTER),
	btnAR /*Wide*/ {{hWnd(), "16x9",		10,		60,		70,		20},
					{hWnd(), "16x10",		10,		85,		70,		20},
	/*Ultrawide*/	{hWnd(), "21x9",		85,		60,		70,		20},
					{hWnd(), "32x9",		85,		85,		70,		20},
					{hWnd(), "48x9",		85,		110,	70,		20},
	/*Portrait*/	{hWnd(), "9x16",		160,	60,		70,		20},
					{hWnd(), "10x16",		160,	85,		70,		20},
					{hWnd(), "9x18",		160,	110,	70,		20},
	/*Square*/		{hWnd(), "1x1",			235,	60,		70,		20},
					{hWnd(), "3x2",			235,	85,		70,		20},
					{hWnd(), "4x3",			235,	110,	70,		20},
					{hWnd(), "5x4",			235,	135,	70,		20}},
	btnCancel		(hWnd(), "Cancel",		10,		165,	142,	20),
	btnOk			(hWnd(), "Ok",			162,	165,	143,	20)
{
	EnableWindow(m_hCaller, FALSE);

	//initializing
	if (strstr(m_sAspRat, "landscape") != NULL)
		btnAllWide.check(true);
	if (strstr(m_sAspRat, "portrait") != NULL)
		btnAllPortrait.check(true);
	for (int i = 0; i < 12; i++)
	{
		char buf[16] = { 0 };
		GetWindowTextA(btnAR[i].hWnd(), buf, 15);
		if (strstr(m_sAspRat, buf) != NULL)
			btnAR[i].check(true);
	}
	
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	centerWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

AspRatPickerWindow::~AspRatPickerWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT AspRatPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->hwndItem == btnAllPortrait.hWnd())
		{
			btnAllPortrait.draw(pDIS);
			return TRUE;
		}
		if (pDIS->hwndItem == btnAllWide.hWnd())
		{
			btnAllWide.draw(pDIS);
			return TRUE;
		}
		for (int i = 0; i < 12; i++)
			if (pDIS->hwndItem == btnAR[i].hWnd())
			{
				btnAR[i].draw(pDIS);
				return TRUE;
			}
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::mainBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if (btnAllWide.isClicked(wParam))
		{
			btnAllWide.check(!btnAllWide.isChecked());
			return 0;
		}
		if (btnAllPortrait.isClicked(wParam))
		{
			btnAllPortrait.check(!btnAllPortrait.isChecked());
			return 0;
		}
		for (int i = 0; i < 12; i++)
			if (btnAR[i].isClicked(wParam))
			{
				btnAR[i].check(!btnAR[i].isChecked());
				return 0;
			}
		if (btnOk.isClicked(wParam))
		{
			strcpy_s(m_sAspRat, 128, "");
			bool empty = true;
			if (btnAllWide.isChecked() || btnAllPortrait.isChecked())
				empty = false;
			int i = 0;
			while (empty && i < 12)
			{
				if (btnAR[i].isChecked())
					empty = false;
				i++;
			}
			if (empty)
			{
				DestroyWindow(hWnd());
				return 0;
			}
			strcpy_s(m_sAspRat, 128, "&ratios=");
			if (btnAllWide.isChecked())
				strcat_s(m_sAspRat, 128, "landscape");
			if (btnAllPortrait.isChecked())
				strcat_s(m_sAspRat, 128, ",portrait");
			for (i = 0; i < 12; i++)
				if (btnAR[i].isChecked())
				{
					strcat_s(m_sAspRat, 128, ",");
					char buf[16] = { 0 };
					GetWindowTextA(btnAR[i].hWnd(), buf, 15);
					strcat_s(m_sAspRat, 128, buf);
				}
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, WindowStyles::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)WindowStyles::mainBkBrush;
	}

	case WM_CTLCOLORBTN:
	return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);

	case WM_SETCURSOR:
	{
		btnAllWide.mouseHovering(wParam);
		btnAllPortrait.mouseHovering(wParam);
		for (int i = 0; i < 12; i++)
			btnAR[i].mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}