#include "ColorPickerButton.h"

#include <Commdlg.h>

#include "IWindow.h"
#include "UIColor.h"

ColorPickerButton::ColorPickerButton(IComponent* pParent, COLORREF color, int x, int y, int width, int height) :
	IHoverable(pParent),
	m_color(color),
	m_brush(CreateSolidBrush(m_color)),
	m_outlinePen(CreatePen(PS_SOLID, 2, UIColor::white))
{
	m_hWnd = CreateWindowExA(0, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
}

ColorPickerButton::~ColorPickerButton()
{
	DeleteObject(m_outlinePen);
	DeleteObject(m_brush);
	DestroyWindow(m_hWnd);
}

bool ColorPickerButton::draw(LPDRAWITEMSTRUCT pDIS) const
{
	if (pDIS->hwndItem != m_hWnd)
		return false;

	FillRect(pDIS->hDC, &pDIS->rcItem, IWindow::Resources::mainBkBrush);
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(pDIS->hDC, m_brush));
	RoundRect(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom, 5, 5);
	SelectObject(pDIS->hDC, oldBrush);

	if (m_hovering)
	{
		HPEN oldPen = static_cast<HPEN>(SelectObject(pDIS->hDC, m_outlinePen));

		MoveToEx(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1, nullptr);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.bottom - 1);
		LineTo(pDIS->hDC, pDIS->rcItem.right - 1, pDIS->rcItem.top + 1);
		LineTo(pDIS->hDC, pDIS->rcItem.left + 1, pDIS->rcItem.top + 1);

		SelectObject(pDIS->hDC, oldPen);

		RECT tmp{};
		tmp.left = pDIS->rcItem.left, tmp.right = tmp.left + 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
		tmp.left = pDIS->rcItem.right, tmp.right = tmp.left - 1;
		tmp.top = pDIS->rcItem.bottom, tmp.bottom = tmp.top - 1;
		FillRect(pDIS->hDC, &tmp, IWindow::Resources::mainBkBrush);
	}

	return true;
}

void ColorPickerButton::click()
{
	COLORREF custom[16]{};
	CHOOSECOLOR cc{};
	cc.hwndOwner = parent()->hWnd();
	cc.lpCustColors = custom;
	cc.rgbResult = m_color;
	cc.Flags = CC_RGBINIT | CC_ANYCOLOR | CC_FULLOPEN;
	cc.lStructSize = sizeof(cc);
	if (!ChooseColor(&cc))
		return;

	m_color = cc.rgbResult;
	DeleteObject(m_brush);
	m_brush = CreateSolidBrush(m_color);
	InvalidateRect(m_hWnd, nullptr, FALSE);
}
