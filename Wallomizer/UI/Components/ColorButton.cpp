#include "ColorButton.h"

#include "resource.h"
#include "IWindow.h"
#include "UIColor.h"

std::uint16_t ColorButton::Resources::s_refCount = 0;
HPEN	ColorButton::Resources::s_checkedPenWhite = nullptr,
		ColorButton::Resources::s_checkedPenBlack = nullptr,
		ColorButton::Resources::s_nullPen = nullptr;
HICON	ColorButton::Resources::s_hICheckWhite = nullptr,
		ColorButton::Resources::s_hICheckBlack = nullptr,
		ColorButton::Resources::s_hIColorEmpty = nullptr;

ColorButton::Resources::Resources()
{
	if (s_refCount++) // Loading icons only if this is the first player creating
		return;
	s_nullPen = CreatePen(PS_NULL, 0, 0);
	s_checkedPenWhite = CreatePen(PS_SOLID, 2, UIColor::white);
	s_checkedPenBlack = CreatePen(PS_SOLID, 2, UIColor::black);
	s_hICheckWhite = static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_WHITE), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	s_hICheckBlack = static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECK_BLACK), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	s_hIColorEmpty = static_cast<HICON>(LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_COLOR_EMPTY), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
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
	m_empty(empty), m_red(red), m_green(green), m_blue(blue),
	m_checkedPenIsWhite(m_red + m_green + m_blue < 255)
{
	m_hWnd = CreateWindowExA(0, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
	m_brush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
}

ColorButton::~ColorButton()
{
	DeleteObject(m_brush);
	DestroyWindow(m_hWnd);
}

void ColorButton::setChecked(bool state)
{
	if (m_checked != state)
	{
		m_checked = state;
		InvalidateRect(m_hWnd, nullptr, FALSE);
	}
}

bool ColorButton::draw(LPDRAWITEMSTRUCT pDIS)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;

	FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
	SelectObject(pDIS->hDC, resources.s_nullPen);
	SelectObject(pDIS->hDC, m_brush);
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	if (m_empty)
		DrawIconEx(pDIS->hDC, 0, 0, resources.s_hIColorEmpty, 0, 0, 0, nullptr, DI_NORMAL);
	if ((m_hovering && !m_checked) || m_checked)
	{
		SelectObject(pDIS->hDC, m_checkedPenIsWhite ? resources.s_checkedPenWhite : resources.s_checkedPenBlack);

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, nullptr);
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
					0, 0, 0, nullptr, DI_NORMAL);

	return true;
}

std::wstring ColorButton::getColor() const
{
	wchar_t buffer[7];
	swprintf_s(buffer, L"%02x%02x%02x", m_red, m_green, m_blue);
	return buffer;
}
