#include <string>

#include "AspRatPickerWindow.h"
#include "MainWindow.h"

AspRatPickerWindow* AspRatPickerWindow::aspRatPickerWindow = nullptr;
char* AspRatPickerWindow::aspRat = nullptr;

LRESULT AspRatPickerWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		btnAllWide = new PushButton(Window(), "All Wide",10,	35,		145,	20);
		btnAllPortrait = new PushButton(Window(), "All Portrait",160,35,70,		20);
		//Wide
		stWide = new Static(Window(), "Wide",			10,		10,		70,		20, SS_CENTER);
		btnAR[0] = new PushButton(Window(), "16x9",		10,		60,		70,		20);
		btnAR[1] = new PushButton(Window(), "16x10",	10,		85,		70,		20);
		//Ultrawide
		stUltrawide = new Static(Window(), "Ultrawide",	85,		10,		70,		20, SS_CENTER);
		btnAR[2] = new PushButton(Window(), "21x9",		85,		60,		70,		20);
		btnAR[3] = new PushButton(Window(), "32x9",		85,		85,		70,		20);
		btnAR[4] = new PushButton(Window(), "48x9",		85,		110,	70,		20);
		//Portrait
		stPortrait = new Static(Window(), "Portrait",	160,	10,		70,		20, SS_CENTER);
		btnAR[5] = new PushButton(Window(), "9x16",		160,	60,		70,		20);
		btnAR[6] = new PushButton(Window(), "10x16",	160,	85,		70,		20);
		btnAR[7] = new PushButton(Window(), "9x18",		160,	110,	70,		20);
		//Square
		stSquare = new Static(Window(), "Square",		235,	10,		70,		20, SS_CENTER);
		btnAR[8] = new PushButton(Window(), "1x1",		235,	60,		70,		20);
		btnAR[9] = new PushButton(Window(), "3x2",		235,	85,		70,		20);
		btnAR[10] = new PushButton(Window(), "4x3",		235,	110,	70,		20);
		btnAR[11] = new PushButton(Window(), "5x4",		235,	135,	70,		20);

		btnCancel = new Button(Window(), "Cancel",		10,		165,	142,	20);
		btnOk = new Button(Window(), "Ok",				162,	165,	143,	20);

		//initializing
		if (strstr(aspRat, "landscape") != NULL)
			btnAllWide->check(true);
		if (strstr(aspRat, "portrait") != NULL)
			btnAllPortrait->check(true);
		for (int i = 0; i < 12; i++)
		{
			char buf[16] = { 0 };
			GetWindowTextA(btnAR[i]->hWnd, buf, 15);
			if (strstr(aspRat, buf) != NULL)
				btnAR[i]->check(true);
		}

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		for (int i = 0; i < 12; i++)
			delete btnAR[i];
		delete btnAllWide, btnAllPortrait;
		delete btnOk, btnCancel;
		delete stUltrawide, stWide, stPortrait, stSquare;
		DeleteObject(font);
		DeleteObject(bkBrush);
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->hwndItem == btnAllPortrait->hWnd)
		{
			btnAllPortrait->draw(pDIS);
			return TRUE;
		}
		if (pDIS->hwndItem == btnAllWide->hWnd)
		{
			btnAllWide->draw(pDIS);
			return TRUE;
		}
		for (int i = 0; i < 12; i++)
			if (pDIS->hwndItem == btnAR[i]->hWnd)
			{
				btnAR[i]->draw(pDIS);
				return TRUE;
			}
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, bkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if COMMANDEVENT(btnAllWide)
		{
			btnAllWide->check(!btnAllWide->isChecked());
			return 0;
		}
		if COMMANDEVENT(btnAllPortrait)
		{
			btnAllPortrait->check(!btnAllPortrait->isChecked());
			return 0;
		}
		for (int i = 0; i < 12; i++)
			if COMMANDEVENT(btnAR[i])
			{
				btnAR[i]->check(!btnAR[i]->isChecked());
				return 0;
			}
		if COMMANDEVENT(btnOk)
		{
			strcpy_s(aspRat, 255, "");
			bool empty = true;
			if (btnAllWide->isChecked() || btnAllPortrait->isChecked())
				empty = false;
			int i = 0;
			while (empty && i < 12)
			{
				if (btnAR[i]->isChecked())
					empty = false;
				i++;
			}
			if (empty)
			{
				DestroyWindow(Window());
				return 0;
			}
			strcpy_s(aspRat, 255, "&ratios=");
			if (btnAllWide->isChecked())
				strcat_s(aspRat, 255, "landscape");
			if (btnAllPortrait->isChecked())
				strcat_s(aspRat, 255, ",portrait");
			for (int i = 0; i < 12; i++)
				if (btnAR[i]->isChecked())
				{
					strcat_s(aspRat, 255, ",");
					char buf[16] = { 0 };
					GetWindowTextA(btnAR[i]->hWnd, buf, 15);
					strcat_s(aspRat, 255, buf);
				}
			DestroyWindow(Window());
			return 0;
		}
		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(Window());
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(129, 193, 193));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)bkBrush;
	}
	return 0;

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void AspRatPickerWindow::windowThread(char* _aspRat, HWND _caller)
{
	if (aspRatPickerWindow)
	{
		SetForegroundWindow(aspRatPickerWindow->Window());
		return;
	}
	EnableWindow(_caller, FALSE);
	aspRat = _aspRat;
	aspRatPickerWindow = new AspRatPickerWindow;
	aspRatPickerWindow->Create("Ratio", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 315, 195, NULL, NULL);
	aspRatPickerWindow->centerWindow(MainWindow::mainWindow->Window());
	ShowWindow(aspRatPickerWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(aspRatPickerWindow->Window(), SW_HIDE);
	aspRatPickerWindow->Destroy();
	delete aspRatPickerWindow;
	aspRatPickerWindow = nullptr;
	EnableWindow(_caller, TRUE);
	SetForegroundWindow(_caller);
}