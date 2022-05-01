#include "PushButton.h"

PushButton::PushButton(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles, COLORREF color1, COLORREF color2)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	m_checkedColorTop = color1;
	m_checkedColorBottom = color2;
}

PushButton::~PushButton()
{
	DestroyWindow(m_hWnd);
}

void PushButton::check(bool state)
{
	if (m_checked != state)
	{
		m_checked = state;
		InvalidateRect(m_hWnd, nullptr, true);
	}
}

bool PushButton::isChecked() const
{
	return m_checked;
}

void PushButton::draw(LPDRAWITEMSTRUCT& pDIS)
{
	int r1 = 70, g1 = 70, b1 = 70;
	int r2 = 40, g2 = 40, b2 = 40;

	if (m_checked)
	{
		r1 = GetRValue(m_checkedColorTop),		g1 = GetGValue(m_checkedColorTop),		b1 = GetBValue(m_checkedColorTop);
		r2 = GetRValue(m_checkedColorBottom),	g2 = GetGValue(m_checkedColorBottom),	b2 = GetBValue(m_checkedColorBottom);
	}
	for (int i = 0; i < pDIS->rcItem.bottom; i++)
	{
		int r, g, b;
		r = r1 + (i * (r2 - r1) / pDIS->rcItem.bottom);
		g = g1 + (i * (g2 - g1) / pDIS->rcItem.bottom);
		b = b1 + (i * (b2 - b1) / pDIS->rcItem.bottom);

		if (!m_checked && m_hovering)
		{
			r = (r + 10) > 255 ? 255 : r + 10;
			g = (g + 10) > 255 ? 255 : r + 10;
			b = (b + 10) > 255 ? 255 : r + 10;
		}

		m_temp.left = 0;
		m_temp.top = i;
		m_temp.right = pDIS->rcItem.right;
		m_temp.bottom = i + 1;

		m_color = CreateSolidBrush(RGB(r, g, b));
		FillRect(pDIS->hDC, &m_temp, m_color);
		DeleteObject(m_color);
	}
	// draw text only if button is horizontal
	if (((double)(pDIS->rcItem.right - pDIS->rcItem.left) / (double)(pDIS->rcItem.bottom - pDIS->rcItem.top)) > 1.1) 
	{
		SetTextColor(pDIS->hDC, m_checked ? RGB(255, 255, 255) : RGB(200, 200, 200));
		SetBkMode(pDIS->hDC, TRANSPARENT);
		SetTextAlign(pDIS->hDC, TA_CENTER);
		char staticText[32];
		staticText[31] = '\0';
		int len = (int)SendMessageA(pDIS->hwndItem, WM_GETTEXT, 31, (LPARAM)staticText);
		TextOutA(pDIS->hDC, (pDIS->rcItem.right - pDIS->rcItem.left) / 2, pDIS->rcItem.left + 3, staticText, len);
	}
}