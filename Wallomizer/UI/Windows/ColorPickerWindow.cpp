#include "ColorPickerWindow.h"

ColorPickerWindow::ColorPickerWindow(HWND hCaller, wchar_t* wsColor) :
	IWindow("Color", "Color Picker Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 405, 195),
	m_hCaller(hCaller),
	m_sColor(wsColor),
	btnClr     {{this, 0x66, 0x00, 0x00,	10,		10,		60,		25},
				{this, 0x99, 0x00, 0x00,	75,		10,		60,		25},
				{this, 0xcc, 0x00, 0x00,	140,	10,		60,		25},
				{this, 0xcc, 0x33, 0x33,	205,	10,		60,		25},
				{this, 0xea, 0x4c, 0x88,	270,	10,		60,		25},
				{this, 0x99, 0x33, 0x99,	335,	10,		60,		25},

				{this, 0x66, 0x33, 0x99,	10,		40,		60,		25},
				{this, 0x33, 0x33, 0x99,	75,		40,		60,		25},
				{this, 0x00, 0x66, 0xcc,	140,	40,		60,		25},
				{this, 0x00, 0x99, 0xcc,	205,	40,		60,		25},
				{this, 0x66, 0xcc, 0xcc,	270,	40,		60,		25},
				{this, 0x77, 0xcc, 0x33,	335,	40,		60,		25},

				{this, 0x66, 0x99, 0x00,	10,		70,		60,		25},
				{this, 0x33, 0x66, 0x00,	75,		70,		60,		25},
				{this, 0x66, 0x66, 0x00,	140,	70,		60,		25},
				{this, 0x99, 0x99, 0x00,	205,	70,		60,		25},
				{this, 0xcc, 0xcc, 0x33,	270,	70,		60,		25},
				{this, 0xff, 0xff, 0x00,	335,	70,		60,		25},

				{this, 0xff, 0xcc, 0x33,	10,		100,	60,		25},
				{this, 0xff, 0x99, 0x00,	75,		100,	60,		25},
				{this, 0xff, 0x66, 0x00,	140,	100,	60,		25},
				{this, 0xcc, 0x66, 0x33,	205,	100,	60,		25},
				{this, 0x99, 0x66, 0x33,	270,	100,	60,		25},
				{this, 0x66, 0x33, 0x00,	335,	100,	60,		25},

				{this, 0x00, 0x00, 0x00,	10,		130,	60,		25},
				{this, 0x99, 0x99, 0x99,	75,		130,	60,		25},
				{this, 0xcc, 0xcc, 0xcc,	140,	130,	60,		25},
				{this, 0xff, 0xff, 0xff,	205,	130,	60,		25},
				{this, 0x42, 0x41, 0x53,	270,	130,	60,		25},
				{this, 0xff, 0xff, 0xff,	335,	130,	60,		25, true}},

	btnCancel	(this, "Cancel",			10,		165,	187,	20),
	btnOk		(this, "Ok",				207,	165,	188,	20)
{
	EnableWindow(m_hCaller, FALSE);

	//initializing
	if (wcsstr(m_sColor, L"&colors=") == NULL)
		btnClr[29].check(true);
	else
		for (int i = 0; i < 29; i++)
		{
			wchar_t sBuf[16] = { 0 };
			btnClr[i].getColor(sBuf, 15);
			if (wcsstr(m_sColor, sBuf) != NULL)
			{
				btnClr[i].check(true);
				break;
			}
		}

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);

	centerWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

ColorPickerWindow::~ColorPickerWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT ColorPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		for (int i = 0; i < 30; i++)
			if (pDIS->hwndItem == btnClr[i].hWnd())
			{
				btnClr[i].draw(pDIS);
				return TRUE;
			}
	}
	return 0;

	case WM_COMMAND:
	{
		for (int i = 0; i < 30; i++)
			if (btnClr[i].isClicked(wParam))
			{
				for (int j = 0; j < 30; j++)
					btnClr[j].check(false);
				btnClr[i].check(true);
				return 0;
			}
		if (btnOk.isClicked(wParam))
		{
			wcscpy_s(m_sColor, 16, L"");
			for (int i = 0; i < 29; i++)
				if (btnClr[i].isChecked())
				{
					wcscpy_s(m_sColor, 16, L"&colors=");
					btnClr[i].getColor(m_sColor, 16);
					break;
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