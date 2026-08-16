#include "ResPickerWindow.h"

#include <algorithm>

ResPickerWindow::ResPickerWindow(IWindow* pOwner, std::wstring& resolution) :
	IWindow(pOwner, "Resolution", "Res Picker Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, 390, 250),
	btnAtLeast	(this, "At least",	10,		10,		180,	20),
	btnExactly	(this, "Exactly",	200,	10,		180,	20),
	stUltrawide	(this, "Ultrawide",	10,		35,		70,		20, SS_CENTER),
	st169		(this, "16:9",		85,		35,		70,		20, SS_CENTER),
	st1610		(this, "16:10",		160,	35,		70,		20, SS_CENTER),
	st43		(this, "4:3",		235,	35,		70,		20, SS_CENTER),
	st54		(this, "5:4",		310,	35,		70,		20, SS_CENTER),
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
	btnCancel	(this, "Cancel",	10,		220,	180,	20),
	btnOk		(this, "Ok",		200,	220,	180,	20),
	m_resolution(resolution)
{
	std::wstring custom = m_resolution;
	const bool exactly = custom.find(resolutionsTag) != std::wstring::npos;

	btnExactly.check(exactly);
	btnAtLeast.check(!exactly);

	std::size_t pos = 0;

	pos = custom.find(resolutionsTag);
	if (pos != std::wstring::npos)
		custom.erase(pos, std::size(resolutionsTag) - 1);

	pos = custom.find(atleastTag);
	if (pos != std::wstring::npos)
		custom.erase(pos, std::size(atleastTag) - 1);

	for(auto& button : btnRes)
	{
		const std::wstring text = button.textW();
		pos = custom.find(text);
		if (pos != std::wstring::npos)
		{
			custom.erase(pos, text.length());
			button.check(true);
		}
	}

	custom.erase(std::remove(custom.begin(), custom.end(), L','), custom.end());

	pos = custom.find(L"x");
	if (pos != std::wstring::npos)
	{
		edWidth.setText(custom.substr(0, pos));
		edHeight.setText(custom.substr(pos + 1));
	}
}

LRESULT ResPickerWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (btnAtLeast.draw(drawItem))
			return TRUE;
		if (btnExactly.draw(drawItem))
			return TRUE;
		for (auto& button : btnRes)
			if (button.draw(drawItem))
				return TRUE;
		break;
	}

	case WM_COMMAND:
	{
		if (btnAtLeast.isClicked(wParam))
		{
			btnAtLeast.check(true);
			btnExactly.check(false);
			int lastChecked = -1;
			for (int i=0; i< std::size(btnRes); i++)
			{
				if (btnRes[i].isChecked())
					lastChecked = i;
				btnRes[i].check(false);
			}
			if (lastChecked != -1)
			{
				edWidth.setText("");
				edHeight.setText("");
				btnRes[lastChecked].check(true);
			}				
			return 0;
		}

		if (btnExactly.isClicked(wParam))
		{
			btnExactly.check(true);
			btnAtLeast.check(false);
			return 0;
		}

		for (auto& button : btnRes)
		{
			if (!button.isClicked(wParam))
				continue;

			button.check(!button.isChecked());
			if (btnAtLeast.isChecked())
			{
				const bool checked = button.isChecked();
				edWidth.setText("");
				edHeight.setText("");
				for (auto& other : btnRes)
					other.check(false);
				button.check(checked);
			}
			return 0;
		}

		const HWND source = reinterpret_cast<HWND>(lParam);
		if ((source == edWidth.hWnd() || source == edHeight.hWnd()) && HIWORD(wParam) == EN_CHANGE && btnAtLeast.isChecked())
		{
			for (auto& button : btnRes)
				button.check(false);
			return 0;
		}

		if (btnOk.isClicked(wParam))
		{
			m_resolution.clear();
			if (btnAtLeast.isChecked())
			{
				for (auto& button : btnRes)
				{
					if (button.isChecked())
					{
						m_resolution += atleastTag;
						m_resolution += button.textW();
						DestroyWindow(hWnd());
						return 0;
					}
				}
				if (!edWidth.isEmpty() && !edHeight.isEmpty())
				{
					m_resolution += atleastTag;
					m_resolution += edWidth.textW();
					m_resolution += L"x";
					m_resolution += edHeight.textW();
					DestroyWindow(hWnd());
					return 0;
				}
			}
			if (btnExactly.isChecked())
			{
				bool first = true;
				for (auto& button : btnRes)
				{
					if (button.isChecked())
					{
						if (first)
						{
							m_resolution = resolutionsTag;
							first = false;
						}
						else
							m_resolution += L",";
						m_resolution += button.textW();
					}
				}
				if (!edWidth.isEmpty() && !edHeight.isEmpty())
				{
					if (first)
						m_resolution = resolutionsTag;
					else
						m_resolution += L",";
	
					m_resolution += edWidth.textW();
					m_resolution += L"x";
					m_resolution += edHeight.textW();
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
		break;
	}
	}

	return RESULT_DEFAULT;
}
