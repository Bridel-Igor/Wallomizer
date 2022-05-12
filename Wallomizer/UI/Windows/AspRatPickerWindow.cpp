#include "AspRatPickerWindow.h"

AspRatPickerWindow::AspRatPickerWindow(HWND hCaller, wchar_t* sAspRat) :
	IWindow("Ratio", "Aspect Ratio Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 315, 195),
	m_hCaller(hCaller),
	m_sAspRat(sAspRat),
	btnAllWide		(this, "All Wide",		10,		35,		145,	20),
	btnAllPortrait	(this, "All Portrait",	160,	35,		70,		20),
	stWide			(this, "Wide",			10,		10,		70,		20, SS_CENTER),
	stUltrawide		(this, "Ultrawide",		85,		10,		70,		20, SS_CENTER),
	stPortrait		(this, "Portrait",		160,	10,		70,		20, SS_CENTER),
	stSquare		(this, "Square",		235,	10,		70,		20, SS_CENTER),
	btnAR /*Wide*/ {{this, "16x9",			10,		60,		70,		20},
					{this, "16x10",			10,		85,		70,		20},
	/*Ultrawide*/	{this, "21x9",			85,		60,		70,		20},
					{this, "32x9",			85,		85,		70,		20},
					{this, "48x9",			85,		110,	70,		20},
	/*Portrait*/	{this, "9x16",			160,	60,		70,		20},
					{this, "10x16",			160,	85,		70,		20},
					{this, "9x18",			160,	110,	70,		20},
	/*Square*/		{this, "1x1",			235,	60,		70,		20},
					{this, "3x2",			235,	85,		70,		20},
					{this, "4x3",			235,	110,	70,		20},
					{this, "5x4",			235,	135,	70,		20}},
	btnCancel		(this, "Cancel",		10,		165,	142,	20),
	btnOk			(this, "Ok",			162,	165,	143,	20)
{
	EnableWindow(m_hCaller, FALSE);

	//initializing
	if (wcsstr(m_sAspRat, L"landscape") != NULL)
		btnAllWide.check(true);
	if (wcsstr(m_sAspRat, L"portrait") != NULL)
		btnAllPortrait.check(true);
	for (int i = 0; i < 12; i++)
	{
		wchar_t buf[16] = { 0 };
		GetWindowTextW(btnAR[i].hWnd(), buf, 15);
		if (wcsstr(m_sAspRat, buf) != NULL)
			btnAR[i].check(true);
	}
	
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
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
			wcscpy_s(m_sAspRat, 128, L"");
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
			wcscpy_s(m_sAspRat, 128, L"&ratios=");
			if (btnAllWide.isChecked())
				wcscat_s(m_sAspRat, 128, L"landscape");
			if (btnAllPortrait.isChecked())
				wcscat_s(m_sAspRat, 128, L",portrait");
			for (i = 0; i < 12; i++)
				if (btnAR[i].isChecked())
				{
					wcscat_s(m_sAspRat, 128, L",");
					wchar_t buf[16] = { 0 };
					GetWindowTextW(btnAR[i].hWnd(), buf, 15);
					wcscat_s(m_sAspRat, 128, buf);
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

	default:
		return RESULT_DEFAULT;
	}
}