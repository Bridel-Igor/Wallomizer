#include <string>

#include "ResPickerWindow.h"
#include "MainWindow.h"

ResPickerWindow* ResPickerWindow::resPickerWindow = nullptr;
char* ResPickerWindow::res = nullptr;

LRESULT ResPickerWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		btnAtLeast = new PushButton(this->hWnd(), "At least",	10,		10,		180,	20);
		btnExactly = new PushButton(this->hWnd(), "Exactly",	200,	10,		180,	20);
		//Ultrawide
		stUltrawide = new Static(this->hWnd(), "Ultrawide",		10,		35,		70,		20, SS_CENTER);
		btnRes[0] = new PushButton(this->hWnd(), "2560x1080",	10,		60,		70,		20);
		btnRes[1] = new PushButton(this->hWnd(), "3440x1440",	10,		85,		70,		20);
		btnRes[2] = new PushButton(this->hWnd(), "3840x1600",	10,		110,	70,		20);
		//16:9
		st169 = new Static(this->hWnd(), "16:9",				85,		35,		70,		20, SS_CENTER);
		btnRes[3] = new PushButton(this->hWnd(), "1280x720",	85,		60,		70,		20);
		btnRes[4] = new PushButton(this->hWnd(), "1600x900",	85,		85,		70,		20);
		btnRes[5] = new PushButton(this->hWnd(), "1920x1080",	85,		110,	70,		20);
		btnRes[6] = new PushButton(this->hWnd(), "2560x1440",	85,		135,	70,		20);
		btnRes[7] = new PushButton(this->hWnd(), "3840x2160",	85,		160,	70,		20);
		//16:10
		st1610 = new Static(this->hWnd(), "16:10",				160,	35,		70,		20, SS_CENTER);
		btnRes[8] = new PushButton(this->hWnd(), "1280x800",	160,	60,		70,		20);
		btnRes[9] = new PushButton(this->hWnd(), "1600x1000",	160,	85,		70,		20);
		btnRes[10] = new PushButton(this->hWnd(), "1920x1200",	160,	110,	70,		20);
		btnRes[11] = new PushButton(this->hWnd(), "2560x1600",	160,	135,	70,		20);
		btnRes[12] = new PushButton(this->hWnd(), "3840x2400",	160,	160,	70,		20);
		//4:3
		st43 = new Static(this->hWnd(), "4:3",					235,	35,		70,		20, SS_CENTER);
		btnRes[13] = new PushButton(this->hWnd(), "1280x960",	235,	60,		70,		20);
		btnRes[14] = new PushButton(this->hWnd(), "1600x1200",	235,	85,		70,		20);
		btnRes[15] = new PushButton(this->hWnd(), "1920x1440",	235,	110,	70,		20);
		btnRes[16] = new PushButton(this->hWnd(), "2560x1920",	235,	135,	70,		20);
		btnRes[17] = new PushButton(this->hWnd(), "3840x2880",	235,	160,	70,		20);
		//5:4
		st54 = new Static(this->hWnd(), "5:4",					310,	35,		70,		20, SS_CENTER);
		btnRes[18] = new PushButton(this->hWnd(), "1280x1024",	310,	60,		70,		20);
		btnRes[19] = new PushButton(this->hWnd(), "1600x1280",	310,	85,		70,		20);
		btnRes[20] = new PushButton(this->hWnd(), "1920x1536",	310,	110,	70,		20);
		btnRes[21] = new PushButton(this->hWnd(), "2560x2048",	310,	135,	70,		20);
		btnRes[22] = new PushButton(this->hWnd(), "3840x3072",	310,	160,	70,		20);
		//custom
		stCustom = new Static(this->hWnd(), "Custom resolution:",20,	190,	110,	20);
		edWidth = new Edit(this->hWnd(), "",					140,	190,	105,	20, ES_NUMBER);
		stX = new Static(this->hWnd(), "x",						245,	190,	20,		20, SS_CENTER);
		edHeight = new Edit(this->hWnd(), "",					265,	190,	105,	20, ES_NUMBER);

		btnCancel = new Button(this->hWnd(), "Cancel",			10,		220,	180,	20);
		btnOk = new Button(this->hWnd(), "Ok",					200,	220,	180,	20);
		
		//initializing
		std::string custom="";
		custom.append(res);
		if (strstr(res, "&resolutions=") != NULL)
			btnExactly->check(true);
		else
			btnAtLeast->check(true);
		for (int i = 0; i < 23; i++)
		{
			char buf[16] = {0};
			GetWindowTextA(btnRes[i]->hWnd(), buf, 15);
			if (strstr(res, buf) != NULL)
			{
				custom.erase(custom.find(buf), strlen(buf));
				btnRes[i]->check(true);
			}
		}
		if (custom.find("&resolutions=") != std::string::npos)
			custom.erase(custom.find("&resolutions="), strlen("&resolutions="));
		if (custom.find("&atleast=") != std::string::npos)
			custom.erase(custom.find("&atleast="), strlen("&atleast="));
		while (custom.find(",") != std::string::npos)
			custom.erase(custom.find(","), strlen(","));
		if (custom.find("x") != std::string::npos)
		{
			char buf[8] = { 0 };
			int n = static_cast<int>(custom.find("x"));
			for (int i = 0; i < n; i++)
				buf[i] = custom[i];
			edWidth->setTextA(buf);
			custom.erase(0, n+1);
			edHeight->setTextA(custom.c_str());
		}

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		for (int i = 0; i < 23; i++)
			delete btnRes[i];

		delete btnAtLeast;
		delete btnExactly;

		delete btnOk;
		delete btnCancel;

		delete stUltrawide;
		delete st169;
		delete st1610;
		delete st43;
		delete st54;
		delete stCustom;
		delete stX;

		delete edWidth;
		delete edHeight;
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
		if (pDIS->hwndItem == btnAtLeast->hWnd())
		{
			btnAtLeast->draw(pDIS);
			return TRUE;
		}
		if (pDIS->hwndItem == btnExactly->hWnd())
		{
			btnExactly->draw(pDIS);
			return TRUE;
		}
		for (int i = 0; i < 23; i++)
			if (pDIS->hwndItem == btnRes[i]->hWnd())
			{
				btnRes[i]->draw(pDIS);
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
		if (btnAtLeast->isClicked(wParam))
		{
			btnAtLeast->check(true);
			btnExactly->check(false);
			int firstChecked = -1;
			for (int i = 22; i >= 0; i--)
			{
				if (btnRes[i]->isChecked() && firstChecked==-1)
				{
					firstChecked = i;
					continue;
				}
				btnRes[i]->check(false);
			}
			if (firstChecked != -1)
			{
				edWidth->setTextA("");
				edHeight->setTextA("");
				btnRes[firstChecked]->check(true);
			}				
			return 0;
		}
		if (btnExactly->isClicked(wParam))
		{
			btnExactly->check(true);
			btnAtLeast->check(false);
			return 0;
		}
		for (int i = 0; i < 23; i++)
			if (btnRes[i]->isClicked(wParam))
			{
				btnRes[i]->check(!btnRes[i]->isChecked());
				if (btnAtLeast->isChecked())
				{
					bool iChecked = btnRes[i]->isChecked();
					edWidth->setTextA("");
					edHeight->setTextA("");
					for (int j = 0; j < 23; j++)
						if (i != j)
							btnRes[j]->check(false);
					btnRes[i]->check(iChecked);
				}
				return 0;
			}
		if (((HWND)lParam == edWidth->hWnd() || (HWND)lParam == edHeight->hWnd()) && HIWORD(wParam) == EN_CHANGE && btnAtLeast->isChecked())
			for (int i = 0; i < 23; i++)
				btnRes[i]->check(false);
		if (btnOk->isClicked(wParam))
		{
			strcpy_s(res, 255, "");
			if (btnAtLeast->isChecked())
			{
				for (int i = 0; i < 23; i++)
				{
					if (btnRes[i]->isChecked())
					{
						char buf[16] = {0};
						GetWindowTextA(btnRes[i]->hWnd(), buf, 15);
						strcpy_s(res, 255, "&atleast=");
						strcat_s(res, 255, buf);
						DestroyWindow(this->hWnd());
						return 0;
					}
				}
				if (!edWidth->isEmpty() && !edHeight->isEmpty())
				{
					char buf[16] = { 0 };
					strcpy_s(res, 255, "&atleast=");
					edWidth->getTextA(buf, 15);
					strcat_s(res, 255, buf);
					strcat_s(res, 255, "x");
					edHeight->getTextA(buf, 15);
					strcat_s(res, 255, buf);
					DestroyWindow(this->hWnd());
					return 0;
				}
			}
			if (btnExactly->isChecked())
			{
				bool first = true;
				for (int i = 0; i < 23; i++)
				{
					if (btnRes[i]->isChecked())
					{
						char buf[16] = { 0 };
						GetWindowTextA(btnRes[i]->hWnd(), buf, 15);
						if (first)
						{
							strcpy_s(res, 255, "&resolutions=");
							first = false;
						}
						else
							strcat_s(res, 255, ",");
						strcat_s(res, 255, buf);
					}
				}
				if (!edWidth->isEmpty() && !edHeight->isEmpty())
				{
					if (first)
						strcpy_s(res, 255, "&resolutions=");
					else
						strcat_s(res, 255, ",");
					char buf[16] = { 0 };
					edWidth->getTextA(buf, 15);
					strcat_s(res, 255, buf);
					strcat_s(res, 255, "x");
					edHeight->getTextA(buf, 15);
					strcat_s(res, 255, buf);
				}
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
		btnAtLeast->mouseHovering(wParam);
		btnExactly->mouseHovering(wParam);
		for (int i = 0; i < 23; i++)
			btnRes[i]->mouseHovering(wParam);
		return FALSE;
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void ResPickerWindow::windowThread(char* _res, HWND _caller)
{
	if (resPickerWindow)
	{
		SetForegroundWindow(resPickerWindow->hWnd());
		return;
	}
	EnableWindow(_caller, FALSE);
	res = _res;
	resPickerWindow = new ResPickerWindow;
	resPickerWindow->Create("Resolution", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 390, 250, NULL, NULL);
	resPickerWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(resPickerWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(resPickerWindow->hWnd(), SW_HIDE);
	resPickerWindow->Destroy();
	delete resPickerWindow;
	resPickerWindow = nullptr;
	EnableWindow(_caller, TRUE);
	SetForegroundWindow(_caller);
}