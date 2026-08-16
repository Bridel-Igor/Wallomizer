#include "AspRatPickerWindow.h"

AspRatPickerWindow::AspRatPickerWindow(IWindow* pOwner, std::wstring& aspRat) :
	IWindow(pOwner, "Ratio", "Aspect Ratio Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, 315, 195),
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
	btnOk			(this, "Ok",			162,	165,	143,	20),
	m_aspRat(aspRat)
{
	if (m_aspRat.find(L"landscape") != std::wstring::npos)
		btnAllWide.check(true);
	if (m_aspRat.find(L"portrait") != std::wstring::npos)
		btnAllPortrait.check(true);

	for (auto& button : btnAR)
		if (m_aspRat.find(button.textW()) != std::wstring::npos)
			button.check(true);
}

LRESULT AspRatPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (btnAllPortrait.draw(drawItem))
			return TRUE;
		if (btnAllWide.draw(drawItem))
			return TRUE;
		for (auto& button : btnAR)
			if (button.draw(drawItem))
				return TRUE;
		break;
	}

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
		for (auto& button : btnAR)
			if (button.isClicked(wParam))
			{
				button.check(!button.isChecked());
				return 0;
			}
		if (btnOk.isClicked(wParam))
		{
			bool empty = true;
			auto append = [&](const std::wstring& value)
				{
					if (!empty)
						m_aspRat += L',';

					m_aspRat += value;
					empty = false;
				};

			m_aspRat = L"&ratios=";
			if (btnAllWide.isChecked())
				append( L"landscape");
			if (btnAllPortrait.isChecked())
				append(L"portrait");
			for (auto& button : btnAR)
				if (button.isChecked())
					append(button.textW());

			if (empty)
				m_aspRat.clear();

			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
		break;
	}
	}

	return RESULT_DEFAULT;
}
