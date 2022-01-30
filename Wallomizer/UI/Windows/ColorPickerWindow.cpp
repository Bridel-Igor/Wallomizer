#include <string>

#include "ColorPickerWindow.h"
#include "MainWindow.h"

ColorPickerWindow* ColorPickerWindow::colorPickerWindow = nullptr;
char* ColorPickerWindow::color = nullptr;

LRESULT ColorPickerWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		btnClr[0] =  new ColorButton(this->hWnd(), 0x66, 0x00, 0x00,	10,		10,		60,		25);
		btnClr[1] =  new ColorButton(this->hWnd(), 0x99, 0x00, 0x00,	75,		10,		60,		25);
		btnClr[2] =  new ColorButton(this->hWnd(), 0xcc, 0x00, 0x00,	140,	10,		60,		25);
		btnClr[3] =  new ColorButton(this->hWnd(), 0xcc, 0x33, 0x33,	205,	10,		60,		25);
		btnClr[4] =  new ColorButton(this->hWnd(), 0xea, 0x4c, 0x88,	270,	10,		60,		25);
		btnClr[5] =  new ColorButton(this->hWnd(), 0x99, 0x33, 0x99,	335,	10,		60,		25);

		btnClr[6] =  new ColorButton(this->hWnd(), 0x66, 0x33, 0x99,	10,		40,		60,		25);
		btnClr[7] =  new ColorButton(this->hWnd(), 0x33, 0x33, 0x99,	75,		40,		60,		25);
		btnClr[8] =  new ColorButton(this->hWnd(), 0x00, 0x66, 0xcc,	140,	40,		60,		25);
		btnClr[9] =  new ColorButton(this->hWnd(), 0x00, 0x99, 0xcc,	205,	40,		60,		25);
		btnClr[10] = new ColorButton(this->hWnd(), 0x66, 0xcc, 0xcc,	270,	40,		60,		25);
		btnClr[11] = new ColorButton(this->hWnd(), 0x77, 0xcc, 0x33,	335,	40,		60,		25);

		btnClr[12] = new ColorButton(this->hWnd(), 0x66, 0x99, 0x00,	10,		70,		60,		25);
		btnClr[13] = new ColorButton(this->hWnd(), 0x33, 0x66, 0x00,	75,		70,		60,		25);
		btnClr[14] = new ColorButton(this->hWnd(), 0x66, 0x66, 0x00,	140,	70,		60,		25);
		btnClr[15] = new ColorButton(this->hWnd(), 0x99, 0x99, 0x00,	205,	70,		60,		25);
		btnClr[16] = new ColorButton(this->hWnd(), 0xcc, 0xcc, 0x33,	270,	70,		60,		25);
		btnClr[17] = new ColorButton(this->hWnd(), 0xff, 0xff, 0x00,	335,	70,		60,		25);

		btnClr[18] = new ColorButton(this->hWnd(), 0xff, 0xcc, 0x33,	10,		100,	60,		25);
		btnClr[19] = new ColorButton(this->hWnd(), 0xff, 0x99, 0x00,	75,		100,	60,		25);
		btnClr[20] = new ColorButton(this->hWnd(), 0xff, 0x66, 0x00,	140,	100,	60,		25);
		btnClr[21] = new ColorButton(this->hWnd(), 0xcc, 0x66, 0x33,	205,	100,	60,		25);
		btnClr[22] = new ColorButton(this->hWnd(), 0x99, 0x66, 0x33,	270,	100,	60,		25);
		btnClr[23] = new ColorButton(this->hWnd(), 0x66, 0x33, 0x00,	335,	100,	60,		25);
		
		btnClr[24] = new ColorButton(this->hWnd(), 0x00, 0x00, 0x00,	10,		130,	60,		25);
		btnClr[25] = new ColorButton(this->hWnd(), 0x99, 0x99, 0x99,	75,		130,	60,		25);
		btnClr[26] = new ColorButton(this->hWnd(), 0xcc, 0xcc, 0xcc,	140,	130,	60,		25);
		btnClr[27] = new ColorButton(this->hWnd(), 0xff, 0xff, 0xff,	205,	130,	60,		25);
		btnClr[28] = new ColorButton(this->hWnd(), 0x42, 0x41, 0x53,	270,	130,	60,		25);
		btnClr[29] = new ColorButton(this->hWnd(), 0xff, 0xff, 0xff,	335,	130,	60,		25, true);

		btnCancel = new Button(this->hWnd(), "Cancel",					10,		165,	187,	20);
		btnOk = new Button(this->hWnd(), "Ok",							207,	165,	188,	20);

		//initializing
		if (strstr(color, "&colors=") == NULL)
			btnClr[29]->check(true);
		else
			for (int i = 0; i < 29; i++)
			{
				char buf[16] = { 0 };
				btnClr[i]->getColor(buf, 15);
				if (strstr(color, buf) != NULL)
				{
					btnClr[i]->check(true);
					break;
				}
			}

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		for (int i = 0; i < 30; i++)
			delete btnClr[i];
		delete btnOk;
		delete btnCancel;
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
		for (int i = 0; i < 30; i++)
			if (pDIS->hwndItem == btnClr[i]->hWnd())
			{
				btnClr[i]->draw(pDIS);
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
		for (int i = 0; i < 30; i++)
			if (btnClr[i]->isClicked(wParam))
			{
				for (int i = 0; i < 30; i++)
					btnClr[i]->check(false);
				btnClr[i]->check(true);
				return 0;
			}
		if (btnOk->isClicked(wParam))
		{
			strcpy_s(color, 16, "");
			for (int i = 0; i < 29; i++)
				if (btnClr[i]->isChecked())
				{
					strcpy_s(color, 16, "&colors=");
					btnClr[i]->getColor(color, 16);
					break;
				}
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnCancel->isClicked(wParam))
		{
			DestroyWindow(this->hWnd());
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		for (int i = 0; i < 30; i++)
			btnClr[i]->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void ColorPickerWindow::windowThread(char* _color, HWND _caller)
{
	if (colorPickerWindow)
	{
		SetForegroundWindow(colorPickerWindow->hWnd());
		return;
	}
	EnableWindow(_caller, FALSE);
	color = _color;
	colorPickerWindow = new ColorPickerWindow;
	colorPickerWindow->Create("Color", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 405, 195, NULL, NULL);
	colorPickerWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(colorPickerWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(colorPickerWindow->hWnd(), SW_HIDE);
	colorPickerWindow->Destroy();
	delete colorPickerWindow;
	colorPickerWindow = nullptr;
	EnableWindow(_caller, TRUE);
	SetForegroundWindow(_caller);
}