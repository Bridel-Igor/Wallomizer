#include "ColorButton.h"
#include "WindowStyles.h"

ColorButton::ColorButton(HWND hParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty):
	m_empty(empty), m_red(red), m_green(green), m_blue(blue)
{
	m_hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	m_brush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
	m_pen = CreatePen(PS_NULL, 0, 0);
	m_checkedPen = CreatePen(PS_SOLID, 2, m_red + m_green + m_blue > 255 ? RGB(0, 0, 0) : RGB(255, 255, 255));
}

ColorButton::~ColorButton()
{
	DeleteObject(m_pen);
	DeleteObject(m_checkedPen);
	DeleteObject(m_brush);
	DestroyWindow(m_hWnd);
}

void ColorButton::check(bool state)
{
	if (m_checked != state)
	{
		m_checked = state;
		InvalidateRect(m_hWnd, nullptr, true);
	}
}

bool ColorButton::isChecked()
{
	return m_checked;
}

void ColorButton::draw(LPDRAWITEMSTRUCT& pDIS)
{
	FillRect(pDIS->hDC, &pDIS->rcItem, WindowStyles::mainBkBrush);
	SelectObject(pDIS->hDC, m_pen);
	SelectObject(pDIS->hDC, m_brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	if (m_empty)
		DrawIconEx(pDIS->hDC, 0, 0, WindowStyles::hIColorEmpty, 0, 0, 0, NULL, DI_NORMAL);
	if ((m_hovering && !m_checked) || m_checked)
	{
		SelectObject(pDIS->hDC, m_checkedPen);

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, NULL);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.top + 1);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1);

		RECT tmp;
		tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
		tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, WindowStyles::mainBkBrush);
	}
	if (m_checked)
		DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
					m_red + m_green + m_blue > 255 ? WindowStyles::hICheckBlack : WindowStyles::hICheckWhite,
					0, 0, 0, NULL, DI_NORMAL);
}

void ColorButton::getColor(char* buffer, int size)
{
	char c[3] = { 0 };
	_itoa_s(m_red, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
	_itoa_s(m_green, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
	_itoa_s(m_blue, c, 16);
	if (strlen(c) == 1)
	{
		c[1] = c[0];
		c[0] = '0';
		c[2] = '\0';
	}
	strcat_s(buffer, size, c);
}