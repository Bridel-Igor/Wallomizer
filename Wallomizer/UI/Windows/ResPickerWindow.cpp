#include <string>

#include "ResPickerWindow.h"

ResPickerWindow::ResPickerWindow(HWND hCaller, wchar_t* wsResolution) :
	IWindow("Resolution", "Res Picker Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 390, 250),
	m_hCaller(hCaller),
	m_wsResolution(wsResolution),
	btnAtLeast	(this, "At least",	10,		10,		180,	20),
	btnExactly	(this, "Exactly",		200,	10,		180,	20),
	stUltrawide	(this, "Ultrawide",	10,		35,		70,		20, SS_CENTER),
	st169		(this, "16:9",		85,		35,		70,		20, SS_CENTER),
	st1610		(this, "16:10",		160,	35,		70,		20, SS_CENTER),
	st43		(this, "4:3",			235,	35,		70,		20, SS_CENTER),
	st54		(this, "5:4",			310,	35,		70,		20, SS_CENTER),
	btnRes	   {{this, "2560x1080",	10,		60,		70,		20}, //Ultrawide
				{this, "3440x1440",	10,		85,		70,		20},
				{this, "3840x1600",	10,		110,	70,		20},
				{this, "1280x720",	85,		60,		70,		20}, //16:9
				{this, "1600x900",	85,		85,		70,		20},
				{this, "1920x1080",	85,		110,	70,		20},
				{this, "2560x1440",	85,		135,	70,		20},
				{this, "3840x2160",	85,		160,	70,		20},
				{this, "1280x800",	160,	60,		70,		20}, //16:10
				{this, "1600x1000",	160,	85,		70,		20},
				{this, "1920x1200",	160,	110,	70,		20},
				{this, "2560x1600",	160,	135,	70,		20},
				{this, "3840x2400",	160,	160,	70,		20},
				{this, "1280x960",	235,	60,		70,		20}, //4:3
				{this, "1600x1200",	235,	85,		70,		20},
				{this, "1920x1440",	235,	110,	70,		20},
				{this, "2560x1920",	235,	135,	70,		20},
				{this, "3840x2880",	235,	160,	70,		20},
				{this, "1280x1024",	310,	60,		70,		20}, //5:4
				{this, "1600x1280",	310,	85,		70,		20},
				{this, "1920x1536",	310,	110,	70,		20},
				{this, "2560x2048",	310,	135,	70,		20},
				{this, "3840x3072",	310,	160,	70,		20}},
	stCustom	(this, "Custom resolution:", 20, 190, 110,	20),
	edWidth		(this, L"",			140,	190,	105,	20, ES_NUMBER),
	stX			(this, "x",			245,	190,	20,		20, SS_CENTER),
	edHeight	(this, L"",			265,	190,	105,	20, ES_NUMBER),
	btnCancel	(this, "Cancel",		10,		220,	180,	20),
	btnOk		(this, "Ok",			200,	220,	180,	20)
{
	EnableWindow(m_hCaller, FALSE);

	//initializing
	std::wstring custom = L"";
	custom.append(m_wsResolution);
	if (wcswcs(m_wsResolution, L"&resolutions=") != NULL)
		btnExactly.check(true);
	else
		btnAtLeast.check(true);
	for (int i = 0; i < 23; i++)
	{
		wchar_t buf[16] = { 0 };
		GetWindowTextW(btnRes[i].hWnd(), buf, 15);
		if (wcswcs(m_wsResolution, buf) != NULL)
		{
			custom.erase(custom.find(buf), wcslen(buf));
			btnRes[i].check(true);
		}
	}
	if (custom.find(L"&resolutions=") != std::string::npos)
		custom.erase(custom.find(L"&resolutions="), wcslen(L"&resolutions="));
	if (custom.find(L"&atleast=") != std::string::npos)
		custom.erase(custom.find(L"&atleast="), wcslen(L"&atleast="));
	while (custom.find(L",") != std::string::npos)
		custom.erase(custom.find(L","), wcslen(L","));
	if (custom.find(L"x") != std::string::npos)
	{
		wchar_t buf[8] = { 0 };
		size_t n = custom.find(L"x");
		for (size_t i = 0; i < n; i++)
			buf[i] = custom[i];
		edWidth.setTextW(buf);
		custom.erase(0, n + 1);
		edHeight.setTextW(custom.c_str());
	}

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	centerWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

ResPickerWindow::~ResPickerWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT ResPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->hwndItem == btnAtLeast.hWnd())
		{
			btnAtLeast.draw(pDIS);
			return TRUE;
		}
		if (pDIS->hwndItem == btnExactly.hWnd())
		{
			btnExactly.draw(pDIS);
			return TRUE;
		}
		for (int i = 0; i < 23; i++)
			if (pDIS->hwndItem == btnRes[i].hWnd())
			{
				btnRes[i].draw(pDIS);
				return TRUE;
			}
	}
	return 0;

	case WM_COMMAND:
	{
		if (btnAtLeast.isClicked(wParam))
		{
			btnAtLeast.check(true);
			btnExactly.check(false);
			int firstChecked = -1;
			for (int i = 22; i >= 0; i--)
			{
				if (btnRes[i].isChecked() && firstChecked==-1)
				{
					firstChecked = i;
					continue;
				}
				btnRes[i].check(false);
			}
			if (firstChecked != -1)
			{
				edWidth.setTextA("");
				edHeight.setTextA("");
				btnRes[firstChecked].check(true);
			}				
			return 0;
		}
		if (btnExactly.isClicked(wParam))
		{
			btnExactly.check(true);
			btnAtLeast.check(false);
			return 0;
		}
		for (int i = 0; i < 23; i++)
			if (btnRes[i].isClicked(wParam))
			{
				btnRes[i].check(!btnRes[i].isChecked());
				if (btnAtLeast.isChecked())
				{
					bool iChecked = btnRes[i].isChecked();
					edWidth.setTextA("");
					edHeight.setTextA("");
					for (int j = 0; j < 23; j++)
						if (i != j)
							btnRes[j].check(false);
					btnRes[i].check(iChecked);
				}
				return 0;
			}
		if (((HWND)lParam == edWidth.hWnd() || (HWND)lParam == edHeight.hWnd()) && HIWORD(wParam) == EN_CHANGE && btnAtLeast.isChecked())
			for (int i = 0; i < 23; i++)
				btnRes[i].check(false);
		if (btnOk.isClicked(wParam))
		{
			wcscpy_s(m_wsResolution, 255, L"");
			if (btnAtLeast.isChecked())
			{
				for (int i = 0; i < 23; i++)
				{
					if (btnRes[i].isChecked())
					{
						wchar_t buf[16] = {0};
						GetWindowTextW(btnRes[i].hWnd(), buf, 15);
						wcscpy_s(m_wsResolution, 255, L"&atleast=");
						wcscat_s(m_wsResolution, 255, buf);
						DestroyWindow(hWnd());
						return 0;
					}
				}
				if (!edWidth.isEmpty() && !edHeight.isEmpty())
				{
					wchar_t buf[16] = { 0 };
					wcscpy_s(m_wsResolution, 255, L"&atleast=");
					edWidth.getTextW(buf, 15);
					wcscat_s(m_wsResolution, 255, buf);
					wcscat_s(m_wsResolution, 255, L"x");
					edHeight.getTextW(buf, 15);
					wcscat_s(m_wsResolution, 255, buf);
					DestroyWindow(hWnd());
					return 0;
				}
			}
			if (btnExactly.isChecked())
			{
				bool first = true;
				for (int i = 0; i < 23; i++)
				{
					if (btnRes[i].isChecked())
					{
						wchar_t buf[16] = { 0 };
						GetWindowTextW(btnRes[i].hWnd(), buf, 15);
						if (first)
						{
							wcscpy_s(m_wsResolution, 255, L"&resolutions=");
							first = false;
						}
						else
							wcscat_s(m_wsResolution, 255, L",");
						wcscat_s(m_wsResolution, 255, buf);
					}
				}
				if (!edWidth.isEmpty() && !edHeight.isEmpty())
				{
					if (first)
						wcscpy_s(m_wsResolution, 255, L"&resolutions=");
					else
						wcscat_s(m_wsResolution, 255, L",");
					wchar_t buf[16] = { 0 };
					edWidth.getTextW(buf, 15);
					wcscat_s(m_wsResolution, 255, buf);
					wcscat_s(m_wsResolution, 255, L"x");
					edHeight.getTextW(buf, 15);
					wcscat_s(m_wsResolution, 255, buf);
				}
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