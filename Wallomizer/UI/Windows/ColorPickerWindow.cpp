#include "ColorPickerWindow.h"

ColorPickerWindow::ColorPickerWindow(IWindow* pOwner, std::wstring& color) :
	IWindow(pOwner, "Color", "Color Picker Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, 405, 195),
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
	btnOk		(this, "Ok",				207,	165,	188,	20),
	m_color(color)
{
	if (m_color.find(L"&colors=") == std::wstring::npos)
		btnClr[29].setChecked(true);
	else
		for (auto& button : btnClr)
			if (m_color.find(button.getColor()) != std::wstring::npos)
			{
				button.setChecked(true);
				break;
			}
}

LRESULT ColorPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		for (auto& button : btnClr)
			if (button.draw(drawItem))
				return TRUE;
		break;
	}

	case WM_COMMAND:
	{
		for (auto& button : btnClr)
		{
			if (!button.isClicked(wParam))
				continue;

			for (auto& other : btnClr)
				other.setChecked(false);

			button.setChecked(true);
			return 0;
		}

		if (btnOk.isClicked(wParam))
		{
			m_color = L"";
			for (auto& button : btnClr)
				if (button.isChecked())
				{
					m_color = L"&colors=";
					m_color += button.getColor();
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
		break;
	}
	}

	return RESULT_DEFAULT;
}