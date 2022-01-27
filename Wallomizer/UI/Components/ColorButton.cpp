#include "ColorButton.h"
#include "WindowStyles.h"
#include "resource.h"

unsigned char ColorButton::s_refCount = 0;
HPEN	ColorButton::s_checkedPenWhite = NULL, 
		ColorButton::s_checkedPenBlack = NULL,
		ColorButton::s_nullPen = NULL;
HICON	ColorButton::s_hICheckWhite = NULL,
		ColorButton::s_hICheckBlack = NULL,
		ColorButton::s_hIColorEmpty = NULL;

ColorButton::ColorButton(HWND hParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty):
	m_empty(empty), m_red(red), m_green(green), m_blue(blue)
{
	m_hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	m_brush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
	m_checkedPenIsWhite = m_red + m_green + m_blue < 255;

	// static resources management
	if (s_refCount == 0)
	{
		s_nullPen = CreatePen(PS_NULL, 0, 0);
		s_checkedPenWhite = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
		s_checkedPenBlack = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		s_hICheckWhite = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_WHITE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		s_hICheckBlack = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_BLACK), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
		s_hIColorEmpty = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_COLOR_EMPTY), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	}
	s_refCount++;
}

ColorButton::~ColorButton()
{
	DeleteObject(m_brush);
	DestroyWindow(m_hWnd);

	// static resources management
	s_refCount--;
	if (s_refCount == 0)
	{
		DeleteObject(s_nullPen);
		s_nullPen = NULL;
		DeleteObject(s_checkedPenWhite);
		s_checkedPenWhite = NULL;
		DeleteObject(s_checkedPenBlack);
		s_checkedPenBlack = NULL;
		DestroyIcon(s_hICheckBlack);
		s_hICheckBlack = NULL;
		DestroyIcon(s_hICheckWhite);
		s_hICheckWhite = NULL;
		DestroyIcon(s_hIColorEmpty);
		s_hIColorEmpty = NULL;
	}
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
	SelectObject(pDIS->hDC, s_nullPen);
	SelectObject(pDIS->hDC, m_brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	if (m_empty)
		DrawIconEx(pDIS->hDC, 0, 0, s_hIColorEmpty, 0, 0, 0, NULL, DI_NORMAL);
	if ((m_hovering && !m_checked) || m_checked)
	{
		SelectObject(pDIS->hDC, m_checkedPenIsWhite ? s_checkedPenWhite : s_checkedPenBlack);

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
					m_checkedPenIsWhite ? s_hICheckWhite : s_hICheckBlack,
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