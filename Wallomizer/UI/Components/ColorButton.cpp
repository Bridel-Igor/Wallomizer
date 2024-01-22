#include "ColorButton.h"
#include "resource.h"
#include "IWindow.h"

unsigned char ColorButton::Resources::s_refCount = 0;
HPEN	ColorButton::Resources::s_checkedPenWhite = NULL,
		ColorButton::Resources::s_checkedPenBlack = NULL,
		ColorButton::Resources::s_nullPen = NULL;
HICON	ColorButton::Resources::s_hICheckWhite = NULL,
		ColorButton::Resources::s_hICheckBlack = NULL,
		ColorButton::Resources::s_hIColorEmpty = NULL;

ColorButton::Resources::Resources()
{
	if (s_refCount++) // Loading icons only if this is the first player creating
		return;
	s_nullPen = CreatePen(PS_NULL, 0, 0);
	s_checkedPenWhite = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	s_checkedPenBlack = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	s_hICheckWhite = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_WHITE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	s_hICheckBlack = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_BLACK), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	s_hIColorEmpty = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_COLOR_EMPTY), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
}

ColorButton::Resources::~Resources()
{
	if (--s_refCount) // Destroying icons only if this is the last player destroying
		return;
	DeleteObject(s_nullPen);
	DeleteObject(s_checkedPenWhite);
	DeleteObject(s_checkedPenBlack);
	DestroyIcon(s_hICheckBlack);
	DestroyIcon(s_hICheckWhite);
	DestroyIcon(s_hIColorEmpty);
}

ColorButton::ColorButton(IComponent* pParent, BYTE red, BYTE green, BYTE blue, int x, int y, int width, int height, bool empty) :
	IHoverable(pParent),
	m_empty(empty), m_red(red), m_green(green), m_blue(blue)
{
	m_hWnd = CreateWindowExA(NULL, TEXT("Button"), "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, m_pParent->hWnd(), m_hMenu, NULL, NULL);
	m_brush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
	m_checkedPenIsWhite = m_red + m_green + m_blue < 255;
}

ColorButton::~ColorButton()
{
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

bool ColorButton::isChecked() const
{
	return m_checked;
}

bool ColorButton::draw(LPDRAWITEMSTRUCT& pDIS)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;

	FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
	SelectObject(pDIS->hDC, resources.s_nullPen);
	SelectObject(pDIS->hDC, m_brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	if (m_empty)
		DrawIconEx(pDIS->hDC, 0, 0, resources.s_hIColorEmpty, 0, 0, 0, NULL, DI_NORMAL);
	if ((m_hovering && !m_checked) || m_checked)
	{
		SelectObject(pDIS->hDC, m_checkedPenIsWhite ? resources.s_checkedPenWhite : resources.s_checkedPenBlack);

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, NULL);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.top + 1);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1);

		RECT tmp = { 0 };
		tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
		tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
	}
	if (m_checked)
		DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
					m_checkedPenIsWhite ? resources.s_hICheckWhite : resources.s_hICheckBlack,
					0, 0, 0, NULL, DI_NORMAL);

	return true;
}

void ColorButton::getColor(wchar_t* buffer, int size)
{
	wchar_t c[3] = { 0 };
	_itow_s(m_red, c, 16);
	if (wcslen(c) == 1)
	{
		c[1] = c[0];
		c[0] = L'0';
		c[2] = L'\0';
	}
	wcscat_s(buffer, size, c);
	_itow_s(m_green, c, 16);
	if (wcslen(c) == 1)
	{
		c[1] = c[0];
		c[0] = L'0';
		c[2] = L'\0';
	}
	wcscat_s(buffer, size, c);
	_itow_s(m_blue, c, 16);
	if (wcslen(c) == 1)
	{
		c[1] = c[0];
		c[0] = L'0';
		c[2] = L'\0';
	}
	wcscat_s(buffer, size, c);
}